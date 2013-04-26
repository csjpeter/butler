/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>
#include <math.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_accountingview.h"
#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"
#include "butler_shopsmodel.h"

SCC TidAccountingWindowTitle = QT_TRANSLATE_NOOP("AccountingView", "Already bought new item");
SCC TidEditItemWindowTitle = QT_TRANSLATE_NOOP("AccountingView", "Editing an existing item");

SCC TidDoneButton = QT_TRANSLATE_NOOP("AccountingView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("AccountingView", "Reset");
SCC TidPrevItemButton = QT_TRANSLATE_NOOP("AccountingView", "Previous item");
SCC TidNextItemButton = QT_TRANSLATE_NOOP("AccountingView", "Next item");

SCC TidBoughtFormCheckBox = QT_TRANSLATE_NOOP("AccountingView", "Bought:");
SCC TidOnStockFormCheckBox = QT_TRANSLATE_NOOP("AccountingView", "On stock:");

SCC TidCategoryEditor = QT_TRANSLATE_NOOP("AccountingView", "Brand or type of ware:");
SCC TidQuantityEditor = QT_TRANSLATE_NOOP("AccountingView", "Quantity:");
SCC TidUnitPriceEditor = QT_TRANSLATE_NOOP("AccountingView", "Unit price:");
SCC TidGrossPriceEditor = QT_TRANSLATE_NOOP("AccountingView", "Gross price:");
SCC TidShopSelector = QT_TRANSLATE_NOOP("AccountingView", "Business partner:");
SCC TidWareSelector = QT_TRANSLATE_NOOP("AccountingView", "Common ware name:");
SCC TidPurchaseDateTimeEditor = QT_TRANSLATE_NOOP("AccountingView", "Date of purchase:");
SCC TidUploadDateTimeEditor = QT_TRANSLATE_NOOP("AccountingView", "Date of upload:");
SCC TidCommentEditor = QT_TRANSLATE_NOOP("AccountingView", "Comments:");
SCC TidWareTags = QT_TRANSLATE_NOOP("AccountingView", "Tags for <i>%1</i> :");

SCC TidMandatoryField = QT_TRANSLATE_NOOP("AccountingView", "mandatory field");
SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("AccountingView", "Please fill at least the mandatory fields.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("AccountingView", "Item is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("AccountingView", "Item is updated.");

AccountingView::AccountingView(const QString & dbname, ItemsModel & model, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(model),
	toolBar(this),
	doneButton(QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(QKeySequence(QKeySequence::Refresh)),
	wareEditor(&waresModel(dbname), Ware::Name),
	shopEditor(&shopsModel(dbname), Shop::Name),
	tagsWidget(dbname),
	lastNumEdited(0),
	lastLastNumEdited(0)
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);
	
	item.bought = true;
	item.purchased = QDateTime::currentDateTime();
	uploadDateTime.setEnabled(false);
	tagsWidget.label.setWordWrap(true);

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevClickedSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextClickedSlot()));
	
	connect(&wareEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&categoryEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&quantityEditor, SIGNAL(valueChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&onStockCheck.box, SIGNAL(stateChanged(int)),
			this, SLOT(updateToolButtonStates()));
	connect(&boughtCheck.box, SIGNAL(stateChanged(int)),
			this, SLOT(updateToolButtonStates()));
	connect(&shopEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&purchaseDateTime.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
			this, SLOT(updateToolButtonStates()));
	connect(&commentEditor.edit, SIGNAL(textChanged()),
			this, SLOT(updateToolButtonStates()));
	connect(&tagsWidget, SIGNAL(selectionChanged()),
			this, SLOT(updateToolButtonStates()));

	connect(&quantityEditor, SIGNAL(valueChanged(double)),
			this, SLOT(quantityValueChangedSlot(double)));
	connect(&unitPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(unitPriceValueChangedSlot(double)));
	connect(&grossPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(grossPriceValueChangedSlot(double)));

	connect(&wareEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(wareNameEditFinishedSlot()));
	connect(&wareEditor.box, SIGNAL(currentIndexChanged(int)),
			this, SLOT(wareNameEditFinishedSlot(int)));

	loadState();
	retranslate();
}

AccountingView::~AccountingView()
{
}

void AccountingView::showEvent(QShowEvent *event)
{
	lastWareName.clear();
	mapToGui();
	if(!cursor.isValid())
		uploadDateTime.edit.setDateTime(QDateTime::currentDateTime());

	PannView::showEvent(event);
	wareEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void AccountingView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void AccountingView::loadState()
{
	QString prefix(cursor.isValid() ? "EditItemView" : "AccountingView");
	PannView::loadState(prefix);
}

void AccountingView::saveState()
{
	QString prefix(cursor.isValid() ? "EditItemView" : "AccountingView");
	PannView::saveState(prefix);
}

void AccountingView::mapToGui()
{
	if(cursor.isValid()){
		item = Item(model.item(cursor.row()));
		shopEditor.setText(item.shop);
	}

	uploadDateTime.edit.setDateTime(item.uploaded);
	purchaseDateTime.edit.setDateTime(item.purchased);

	wareEditor.setText(item.name);
	wareNameEditFinishedSlot();
	categoryEditor.setText(item.category);

	quantityEditor.blockSignals(true);
	quantityEditor.setValue(item.quantity);
	quantityEditor.blockSignals(false);

	commentEditor.edit.setText(item.comment);

	unitPriceEditor.blockSignals(true);
	unitPriceEditor.setValue((0.001 <= item.quantity) ? item.price / item.quantity : 0);
	unitPriceEditor.blockSignals(false);

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.setValue(item.price);
	grossPriceEditor.blockSignals(false);

	onStockCheck.box.setCheckState(item.onStock ? Qt::Checked : Qt::Unchecked);
	boughtCheck.box.setCheckState(item.bought ? Qt::Checked : Qt::Unchecked);

	updateToolButtonStates();
}

void AccountingView::mapFromGui()
{
	item.uploaded = uploadDateTime.edit.dateTime();

	item.shop = shopEditor.text();
	item.name = wareEditor.text();
	item.category = categoryEditor.text();
	item.quantity = quantityEditor.value();
	item.comment = commentEditor.edit.toPlainText();

	item.bought = (boughtCheck.box.checkState() == Qt::Checked);
	item.price = grossPriceEditor.value();
	item.purchased = purchaseDateTime.edit.dateTime();

	item.onStock = (onStockCheck.box.checkState() == Qt::Checked);

	ware.name = item.name;
	ware.tags = tagsWidget.selectedTags();
	if(categoryEditor.text().size() && !ware.categories.has(item.category))
		ware.categories.add(new QString(categoryEditor.text()));
}

void AccountingView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void AccountingView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}

void AccountingView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditItemWindowTitle));
	else
		setWindowTitle(tr(TidAccountingWindowTitle));

	wareEditor.label.setText(tr(TidWareSelector));
	wareEditor.editor.setPlaceholderText(tr(TidMandatoryField));
	categoryEditor.label.setText(tr(TidCategoryEditor));
	quantityEditor.label.setText(tr(TidQuantityEditor));
	unitPriceEditor.label.setText(tr(TidUnitPriceEditor));
	grossPriceEditor.label.setText(tr(TidGrossPriceEditor));
	shopEditor.label.setText(tr(TidShopSelector));
	purchaseDateTime.label.setText(tr(TidPurchaseDateTimeEditor));
	uploadDateTime.label.setText(tr(TidUploadDateTimeEditor));
	commentEditor.label.setText(tr(TidCommentEditor));
	onStockCheck.label.setText(tr(TidOnStockFormCheckBox));
	boughtCheck.label.setText(tr(TidBoughtFormCheckBox));

	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));

	relayout();
}

void AccountingView::applyLayout(LayoutState state, bool test)
{
	(void)state;

	delete layout();

	HLayout * toolLayout = new HLayout;
	toolLayout->addWidget(&doneButton);
	toolLayout->addWidget(&resetButton);
	toolLayout->addWidget(&prevButton);
	toolLayout->addWidget(&nextButton);
	toolBar.setLayout(toolLayout);

	setToolBar(&toolBar);

	HLayout * hlayout = new HLayout;
	hlayout->addWidget(&grossPriceEditor);
	hlayout->addStretch();
	hlayout->addWidget(&quantityEditor);
	hlayout->addStretch();
	hlayout->addWidget(&unitPriceEditor);

	HLayout * h2layout = new HLayout;
	h2layout->addWidget(&onStockCheck);
	h2layout->addStretch();
	h2layout->addWidget(&boughtCheck);
	h2layout->addStretch();

	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addLayout(hlayout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&wareEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&categoryEditor);
	mainLayout->addStretch(0);
	mainLayout->addLayout(h2layout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&shopEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&purchaseDateTime);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&uploadDateTime);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&commentEditor);
	mainLayout->addStretch(0);
	if(!test){
		mainLayout->addWidget(&tagsWidget);
		mainLayout->addStretch(0);
	}

	setLayout(mainLayout);
	updateGeometry();
}

void AccountingView::relayout()
{
	LayoutState newState = LayoutState::Expanding;
	doneButton.setText(tr(TidDoneButton));
	resetButton.setText(tr(TidResetButton));
	prevButton.setText(tr(TidPrevItemButton));
	nextButton.setText(tr(TidNextItemButton));

/*	if(width() < sizeHint().width())*/{
		newState = LayoutState::Wide;
		wareEditor.wideLayout();
		categoryEditor.wideLayout();
		quantityEditor.wideLayout();
		unitPriceEditor.wideLayout();
		grossPriceEditor.wideLayout();
		shopEditor.wideLayout();
		purchaseDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout(newState, true);
	}
	if(width() < sizeHint().width()){
		newState = LayoutState::Medium;
		doneButton.setText(trMed(TidDoneButton));
		resetButton.setText(trMed(TidResetButton));
		prevButton.setText(trMed(TidPrevItemButton));
		nextButton.setText(trMed(TidNextItemButton));
		wareEditor.wideLayout();
		categoryEditor.wideLayout();
		quantityEditor.narrowLayout();
		unitPriceEditor.narrowLayout();
		grossPriceEditor.narrowLayout();
		shopEditor.wideLayout();
		purchaseDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout(newState, true);
	}
	if(width() < sizeHint().width()){
		newState = LayoutState::Narrow;
		doneButton.setText(trShort(TidDoneButton));
		resetButton.setText(trShort(TidResetButton));
		prevButton.setText(trShort(TidPrevItemButton));
		nextButton.setText(trShort(TidNextItemButton));
		wareEditor.narrowLayout();
		categoryEditor.narrowLayout();
		quantityEditor.narrowLayout();
		unitPriceEditor.narrowLayout();
		grossPriceEditor.narrowLayout();
		shopEditor.narrowLayout();
		purchaseDateTime.narrowLayout();
		uploadDateTime.narrowLayout();
		commentEditor.narrowLayout();
		applyLayout(newState, true);
	}

	applyLayout(newState);
	updateToolButtonStates();
}

void AccountingView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	boughtCheck.show();
	setWindowTitle(tr(TidEditItemWindowTitle));
	mapToGui();
}

void AccountingView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void AccountingView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void AccountingView::saveSlot()
{
	mapFromGui();

	/* Add shop if not yet known. */
	ShopsModel &sm = shopsModel(dbname);
	int i = sm.index(shopEditor.text());
	if(i == -1){
		Shop shop;
		shop.name = shopEditor.text();
		sm.addNew(shop);
	}

	/* Add/update ware if neccessary. */
	WaresModel &wm = waresModel(dbname);
	i = wm.index(ware.name);
	if(i == -1)
		wm.addNew(ware);
	else if(wm.ware(i) != ware)
		wm.update(i, ware);

	if(cursor.isValid()){
		if(model.item(cursor.row()) != item)
			model.update(cursor.row(), item);
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(item);

		item = Item();
		item.uploaded = QDateTime::currentDateTime();
		ware = Ware();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		wareEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void AccountingView::resetSlot()
{
	mapToGui();
	tagsWidget.setTags(ware.tags);
}

void AccountingView::quantityValueChangedSlot(double q)
{
	if(lastNumEdited != &quantityEditor){
		lastLastNumEdited = lastNumEdited;
		lastNumEdited = &quantityEditor;
	}

	double u = unitPriceEditor.value();
	double g = grossPriceEditor.value();

	if((lastNumEdited == &quantityEditor && lastLastNumEdited == &grossPriceEditor) ||
			lastNumEdited == &grossPriceEditor){
		u = (q < 0.001) ? 0 : g / q;
		unitPriceEditor.blockSignals(true);
		unitPriceEditor.setValue(u);
		unitPriceEditor.blockSignals(false);
	} else {
		grossPriceEditor.blockSignals(true);
		grossPriceEditor.setValue(u * q);
		grossPriceEditor.blockSignals(false);
	}

	updateToolButtonStates();
}

void AccountingView::unitPriceValueChangedSlot(double u)
{
	if(lastNumEdited != &unitPriceEditor){
		lastLastNumEdited = lastNumEdited;
		lastNumEdited = &unitPriceEditor;
	}

	double q = quantityEditor.value();
	double g = grossPriceEditor.value();

	if((lastNumEdited == &unitPriceEditor && lastLastNumEdited == &grossPriceEditor) ||
			lastNumEdited == &grossPriceEditor){
		q = (u < 0.01) ? 0 : g / u;
		quantityEditor.blockSignals(true);
		quantityEditor.setValue(q);
		quantityEditor.blockSignals(false);
	} else {
		grossPriceEditor.blockSignals(true);
		grossPriceEditor.setValue(u * q);
		grossPriceEditor.blockSignals(false);
	}

	updateToolButtonStates();
}

void AccountingView::grossPriceValueChangedSlot(double g)
{
	if(lastNumEdited != &grossPriceEditor){
		lastLastNumEdited = lastNumEdited;
		lastNumEdited = &grossPriceEditor;
	}

	double q = quantityEditor.value();
	double u = unitPriceEditor.value();

	if((lastNumEdited == &grossPriceEditor && lastLastNumEdited == &unitPriceEditor) ||
			lastNumEdited == &unitPriceEditor){
		q = (u < 0.01) ? 0 : g / u;
		quantityEditor.blockSignals(true);
		quantityEditor.setValue(q);
		quantityEditor.blockSignals(false);
	} else {
		u = (q < 0.001) ? 0 : g / q;
		unitPriceEditor.blockSignals(true);
		unitPriceEditor.setValue(u);
		unitPriceEditor.blockSignals(false);
	}

	updateToolButtonStates();
}

void AccountingView::updateToolButtonStates()
{
	bool modified = !(
			item.shop == shopEditor.text() &&
			item.name == wareEditor.text() &&
			item.category == categoryEditor.text() &&
			fabs(item.quantity - quantityEditor.value()) < 0.001 &&
			item.comment == commentEditor.edit.toPlainText() &&
			item.bought == (boughtCheck.box.checkState() == Qt::Checked) &&
			fabs(item.price - grossPriceEditor.value()) < 0.01 &&
			item.purchased == purchaseDateTime.edit.dateTime() &&
			item.onStock == (onStockCheck.box.checkState() == Qt::Checked)
			);

	/* tag states might have changed for ware */
	if(tagsWidget.selectedTags() != ware.tags)
		modified = true;

	bool mandatoriesGiven = wareEditor.editor.text().size();

	prevButton.setVisible(!modified && cursor.isValid() && 0 < cursor.row());
	nextButton.setVisible(!modified && cursor.isValid() && cursor.row() < model.rowCount()-1);
	doneButton.setVisible(mandatoriesGiven && modified);
	resetButton.setVisible(modified);

	if(modified){
		if(!mandatoriesGiven)
			toolBar.setInfo(tr(TidInfoMandatoryFields));
		else
			toolBar.clearInfo();
	}
}

void AccountingView::wareNameEditFinishedSlot()
{
	if(wareEditor.editor.text() == lastWareName)
		return;

	lastWareName = wareEditor.editor.text();

	QString lastCat = categoryEditor.text();
	categoryEditor.box.clear();

	WaresModel &wm = waresModel(dbname);
	int i = wm.index(lastWareName);
	if(i == -1)
		ware = Ware();
	else
		ware = wm.ware(i);

	quantityEditor.setSuffix(ware.unit);
	tagsWidget.setTags(ware.tags);

	QString cats = WaresModel::categoriesToString(ware.categories);
	categoryEditor.box.addItem(lastCat);
	categoryEditor.box.addItems(cats.split(", ", QString::SkipEmptyParts));
	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));
}

/* We need this for cases when keybaord focus was not used,
 * just a mouse selection from the drop down popup. */
void AccountingView::wareNameEditFinishedSlot(int)
{
	wareNameEditFinishedSlot();
}
