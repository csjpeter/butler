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

AccountingView::AccountingView(const QString & dbname, ItemsModel & model, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(model),
	backButton(QKeySequence(Qt::ALT + Qt::Key_Escape)),
	doneButton(QKeySequence(Qt::ALT + Qt::Key_Return)),
	wareEditor(&waresModel(dbname), Ware::Name),
	shopEditor(&shopsModel(dbname), Shop::Name),
	tagsWidget(dbname),
	lastNumEdited(0),
	lastLastNumEdited(0)
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);
	
	item.bought = true;
	uploadDateTime.setEnabled(false);

	connect(&backButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
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
	connect(&quantityEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(quantityEditFinishedSlot()));

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
		purchaseDateTime.edit.setDateTime(item.purchased);
		shopEditor.setText(item.shop);
	}

	uploadDateTime.edit.setDateTime(item.uploaded);

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
		setWindowTitle(qtTrId(TidEditItemWindowTitle));
	else
		setWindowTitle(qtTrId(TidAccountingWindowTitle));

	backButton.setText(qtTrId(TidBackButtonLabel));
	doneButton.setText(qtTrId(TidDoneButtonLabel));
	prevButton.setText(qtTrId(TidPrevItemButtonLabel));
	nextButton.setText(qtTrId(TidNextItemButtonLabel));
	wareEditor.label.setText(qtTrId(TidWareSelectorLabel));
	wareEditor.editor.setPlaceholderText(qtTrId(TidWareSelectorPlaceholder));
	categoryEditor.label.setText(qtTrId(TidCategoryEditorLabel));
	quantityEditor.label.setText(qtTrId(TidQuantityEditorLabel));
	quantityEditor.editor.setPlaceholderText(qtTrId(TidQuantityEditorPlaceholder));
	unitPriceEditor.label.setText(qtTrId(TidUnitPriceEditorLabel));
	grossPriceEditor.label.setText(qtTrId(TidGrossPriceEditorLabel));
	shopEditor.label.setText(qtTrId(TidShopSelectorLabel));
	purchaseDateTime.label.setText(qtTrId(TidPurchaseDateTimeEditorLabel));
	uploadDateTime.label.setText(qtTrId(TidUploadDateTimeEditorLabel));
	commentEditor.label.setText(qtTrId(TidCommentEditorLabel));
	onStockCheck.label.setText(qtTrId(TidOnStockFormCheckBoxLabel));
	boughtCheck.label.setText(qtTrId(TidBoughtFormCheckBoxLabel));

	tagsWidget.label.setText(qtTrId(TidItemViewTagsWidgetLabel).arg(ware.name));

	relayout();
}

enum class ViewState {
	Wide,
	Medium,
	Narrow
};

void AccountingView::applyLayout(bool test)
{
	delete layout();

	HLayout * toolLayout = new HLayout;
	if(prevButton.isEnabled())
		toolLayout->addWidget(&prevButton);
	if(nextButton.isEnabled())
		toolLayout->addWidget(&nextButton);
	toolLayout->addStretch(0);
	toolLayout->addWidget(&infoLabel, 1);
	toolLayout->addStretch(0);
	toolLayout->addWidget(&doneButton);
	toolLayout->addWidget(&backButton);
	delete toolBar.layout();
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
	ViewState newState = ViewState::Wide;
	QSize newSize;

	switch(newState) {
		case ViewState::Wide :
			prevButton.setEnabled(true);
			nextButton.setEnabled(true);
			wareEditor.wideLayout();
			categoryEditor.wideLayout();
			quantityEditor.wideLayout();
			unitPriceEditor.wideLayout();
			grossPriceEditor.wideLayout();
			shopEditor.wideLayout();
			purchaseDateTime.wideLayout();
			uploadDateTime.wideLayout();
			commentEditor.wideLayout();
			applyLayout(true);
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Medium :
			prevButton.setEnabled(true);
			nextButton.setEnabled(true);
			wareEditor.wideLayout();
			categoryEditor.wideLayout();
			quantityEditor.narrowLayout();
			unitPriceEditor.narrowLayout();
			grossPriceEditor.narrowLayout();
			shopEditor.wideLayout();
			purchaseDateTime.wideLayout();
			uploadDateTime.wideLayout();
			commentEditor.wideLayout();
			applyLayout(true);
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Narrow :
			prevButton.setEnabled(false); prevButton.hide(); prevButton.setParent(0);
			nextButton.setEnabled(false); nextButton.hide(); nextButton.setParent(0);
			wareEditor.narrowLayout();
			categoryEditor.narrowLayout();
			quantityEditor.narrowLayout();
			unitPriceEditor.narrowLayout();
			grossPriceEditor.narrowLayout();
			shopEditor.narrowLayout();
			purchaseDateTime.narrowLayout();
			uploadDateTime.narrowLayout();
			commentEditor.narrowLayout();
			applyLayout(true);
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
			break;
	}
	applyLayout(false);
	updateToolButtonStates();
}

void AccountingView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	boughtCheck.show();
	setWindowTitle(qtTrId(TidEditItemWindowTitle));
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
		infoLabel.setText(qtTrId(TidItemEditingSavedInfoLabel));
		if(!prevButton.isEnabled() || !nextButton.isEnabled())
			accept();
	} else {
		model.addNew(item);

		item = Item();
		item.uploaded = QDateTime::currentDateTime();
		ware = Ware();
		mapToGui();
		infoLabel.setText(qtTrId(TidAccountingSavedInfoLabel));
		wareEditor.editor.setFocus(Qt::OtherFocusReason);
	}
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
			item.quantity == quantityEditor.value() &&
			item.comment == commentEditor.edit.toPlainText() &&
			item.bought == (boughtCheck.box.checkState() == Qt::Checked) &&
			item.price == grossPriceEditor.value() &&
			item.purchased == purchaseDateTime.edit.dateTime() &&
			item.onStock == (onStockCheck.box.checkState() == Qt::Checked) &&
			item == Item(model.item(cursor.row()))
			);

	/* tag states might have changed for ware */
	if(tagsWidget.selectedTags() != ware.tags)
		modified = true;

	bool mandatoriesGiven =
		wareEditor.editor.text().size() &&
		0.001 <= quantityEditor.value();

	prevButton.setVisible(!modified && cursor.isValid() &&
			0 < cursor.row() && prevButton.isEnabled());
	nextButton.setVisible(!modified && cursor.isValid() &&
			cursor.row() < model.rowCount()-1 && nextButton.isEnabled());
	doneButton.setVisible(mandatoriesGiven && modified);

	if(modified){
		if(!mandatoriesGiven)
			infoLabel.setText(qtTrId(TidFillMandatoryFieldsInfoLabel));
		else if(infoLabel.text().size())
			infoLabel.setText("");
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
	tagsWidget.label.setText(qtTrId(TidItemViewTagsWidgetLabel).arg(ware.name));
}

/* We need this for cases when keybaord focus was not used,
 * just a mouse selection from the drop down popup. */
void AccountingView::wareNameEditFinishedSlot(int)
{
	wareNameEditFinishedSlot();
}

void AccountingView::quantityEditFinishedSlot()
{
	if(quantityEditor.value() < 0.001)
		quantityEditor.editor.setText("");
}
