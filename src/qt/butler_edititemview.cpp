/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <float.h>
#include <math.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_edititemview.h"
#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"
#include "butler_partnersmodel.h"

SCC TidContext = "EditItemView";

SCC TidNewItemWindowTitle = QT_TRANSLATE_NOOP("EditItemView", "Already bought new item");
SCC TidEditItemWindowTitle = QT_TRANSLATE_NOOP("EditItemView", "Editing an existing item");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditItemView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditItemView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditItemView", "Previous item");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditItemView", "Next item");

SCC TidBoughtFormCheckBox = QT_TRANSLATE_NOOP("EditItemView", "Bought:");
SCC TidOnStockFormCheckBox = QT_TRANSLATE_NOOP("EditItemView", "On stock:");

SCC TidTypeEditor = QT_TRANSLATE_NOOP("EditItemView", "Brand or type of ware:");
SCC TidQuantityEditor = QT_TRANSLATE_NOOP("EditItemView", "Quantity:");
SCC TidUnitPriceEditor = QT_TRANSLATE_NOOP("EditItemView", "Unit price:");
SCC TidGrossPriceEditor = QT_TRANSLATE_NOOP("EditItemView", "Gross price:");
SCC TidPartnerSelector = QT_TRANSLATE_NOOP("EditItemView", "Business partner:");
SCC TidWareSelector = QT_TRANSLATE_NOOP("EditItemView", "Common ware name:");
SCC TidPurchaseDateTimeEditor = QT_TRANSLATE_NOOP("EditItemView", "Date of purchase:");
SCC TidUploadDateTimeEditor = QT_TRANSLATE_NOOP("EditItemView", "Date of upload:");
SCC TidCommentEditor = QT_TRANSLATE_NOOP("EditItemView", "Comments:");
SCC TidWareTags = QT_TRANSLATE_NOOP("EditItemView", "Tags for <i>%1</i> :");

SCC TidMandatoryField = QT_TRANSLATE_NOOP("EditItemView", "mandatory field");
SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditItemView", "Please fill at least the mandatory fields.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditItemView", "Item is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditItemView", "Item is updated.");

EditItemView * EditItemView::newItemViewFactory(const QString & dbname)
{
	csjp::Object<CustomModel> ownModel = customModel(dbname);
	EditItemView * view = new EditItemView(dbname, *ownModel);
	view->ownModel = ownModel.release();
	return view;
}

EditItemView::EditItemView(const QString & dbname, ItemsModel & model, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(model),
	ownModel(NULL),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right)),
	wareEditor(&waresModel(dbname), Ware::Name),
	partnerEditor(&partnersModel(dbname), Partner::Name),
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

	toolBar.addToolWidget(doneButton);
	toolBar.addToolWidget(resetButton);
	footerBar.addToolWidget(prevButton);
	footerBar.addToolWidget(nextButton);
	footerBar.expanding = true;
	footerBar.spacerEnd = false;

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevClickedSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextClickedSlot()));
	
	connect(&wareEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&typeEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&quantityEditor, SIGNAL(valueChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&onStockCheck.box, SIGNAL(stateChanged(int)),
			this, SLOT(updateToolButtonStates()));
	connect(&boughtCheck.box, SIGNAL(stateChanged(int)),
			this, SLOT(updateToolButtonStates()));
	connect(&partnerEditor.box, SIGNAL(editTextChanged(const QString &)),
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

	setupView();
	retranslate();
	loadState();
}

EditItemView::~EditItemView()
{
	if(ownModel)
		delete ownModel;
}

void EditItemView::showEvent(QShowEvent *event)
{
	lastWareName.clear();
	mapToGui();
	if(!cursor.isValid())
		uploadDateTime.edit.setDateTime(QDateTime::currentDateTime());

	PannView::showEvent(event);
	grossPriceEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void EditItemView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void EditItemView::loadState()
{
	QString prefix(cursor.isValid() ? "EditItemView" : "EditItemView");
	PannView::loadState(prefix);
}

void EditItemView::saveState()
{
	QString prefix(cursor.isValid() ? "EditItemView" : "EditItemView");
	PannView::saveState(prefix);
}

void EditItemView::mapToGui()
{
	if(cursor.isValid()){
		item = Item(model.item(cursor.row()));
		partnerEditor.setText(item.partner);
	}

	uploadDateTime.edit.setDateTime(item.uploaded);
	purchaseDateTime.edit.setDateTime(item.purchased);

	wareEditor.setText(item.name);
	wareNameEditFinishedSlot();
	typeEditor.setText(item.type);

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

void EditItemView::mapFromGui()
{
	item.uploaded = uploadDateTime.edit.dateTime();

	item.partner = partnerEditor.text();
	item.name = wareEditor.text();
	item.type = typeEditor.text();
	item.quantity = quantityEditor.value();
	item.comment = commentEditor.edit.toPlainText();

	item.bought = (boughtCheck.box.checkState() == Qt::Checked);
	item.price = grossPriceEditor.value();
	item.purchased = purchaseDateTime.edit.dateTime();

	item.onStock = (onStockCheck.box.checkState() == Qt::Checked);

	ware.name = item.name;
	ware.tags = tagsWidget.selectedTags();
	if(typeEditor.text().size() && !ware.types.has(item.type))
		ware.types.add(new Text(typeEditor.text()));
}

void EditItemView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void EditItemView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}

void EditItemView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditItemWindowTitle));
	else
		setWindowTitle(tr(TidNewItemWindowTitle));

	wareEditor.label.setText(tr(TidWareSelector));
	wareEditor.editor.setPlaceholderText(tr(TidMandatoryField));
	typeEditor.label.setText(tr(TidTypeEditor));
	quantityEditor.label.setText(tr(TidQuantityEditor));
	unitPriceEditor.label.setText(tr(TidUnitPriceEditor));
	grossPriceEditor.label.setText(tr(TidGrossPriceEditor));
	partnerEditor.label.setText(tr(TidPartnerSelector));
	purchaseDateTime.label.setText(tr(TidPurchaseDateTimeEditor));
	uploadDateTime.label.setText(tr(TidUploadDateTimeEditor));
	commentEditor.label.setText(tr(TidCommentEditor));
	onStockCheck.label.setText(tr(TidOnStockFormCheckBox));
	boughtCheck.label.setText(tr(TidBoughtFormCheckBox));

	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));

	relayout();
}

void EditItemView::applyLayout(bool test)
{
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
	mainLayout->addWidget(&typeEditor);
	mainLayout->addStretch(0);
	mainLayout->addLayout(h2layout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&partnerEditor);
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

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditItemView::relayout()
{
	{
		wareEditor.wideLayout();
		typeEditor.wideLayout();
		quantityEditor.wideLayout();
		unitPriceEditor.wideLayout();
		grossPriceEditor.wideLayout();
		partnerEditor.wideLayout();
		purchaseDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout( true);
	}
	if(width() < sizeHint().width()){
		wareEditor.wideLayout();
		typeEditor.wideLayout();
		quantityEditor.narrowLayout();
		unitPriceEditor.narrowLayout();
		grossPriceEditor.narrowLayout();
		partnerEditor.wideLayout();
		purchaseDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout(true);
	}
	if(width() < sizeHint().width()){
		wareEditor.narrowLayout();
		typeEditor.narrowLayout();
		quantityEditor.narrowLayout();
		unitPriceEditor.narrowLayout();
		grossPriceEditor.narrowLayout();
		partnerEditor.narrowLayout();
		purchaseDateTime.narrowLayout();
		uploadDateTime.narrowLayout();
		commentEditor.narrowLayout();
		applyLayout(true);
	}

	applyLayout();
	updateToolButtonStates();
}

void EditItemView::updateToolButtonStates()
{
	bool modified = !(
			item.partner == partnerEditor.text() &&
			item.name == wareEditor.text() &&
			item.type == typeEditor.text() &&
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

	footerBar.show(); /* We cant set visible status for a widget having hidden parent. */
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

	toolBar.updateButtons();
	footerBar.updateButtons();
}

void EditItemView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	boughtCheck.show();
	setWindowTitle(tr(TidEditItemWindowTitle));
	mapToGui();
}

void EditItemView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditItemView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditItemView::saveSlot()
{
	mapFromGui();

	/* Add partner if not yet known. */
	PartnersModel & sm = partnersModel(dbname);
	int i = sm.index(partnerEditor.text());
	if(i == -1){
		Partner partner;
		partner.name = partnerEditor.text();
		partner.company = partnerEditor.text();
		/* Add company if not yet known. */
		CompaniesModel & cm = companiesModel(dbname);
		int i = cm.index(partner.company);
		if(i == -1){
			Company company;
			company.name = partner.company;
			cm.addNew(company);
		}
		sm.addNew(partner);
	}

	/* Add/update ware if neccessary. */
	WaresModel & wm = waresModel(dbname);
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
		item.bought = true;
		item.uploaded = QDateTime::currentDateTime();
		ware = Ware();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		grossPriceEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void EditItemView::resetSlot()
{
	mapToGui();
	tagsWidget.setTags(ware.tags);
}

void EditItemView::quantityValueChangedSlot(double q)
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

void EditItemView::unitPriceValueChangedSlot(double u)
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

void EditItemView::grossPriceValueChangedSlot(double g)
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

void EditItemView::wareNameEditFinishedSlot()
{
	if(lastWareName == wareEditor.editor.text())
		return;

	lastWareName = wareEditor.editor.text();

	QString lastCat = typeEditor.text();
	typeEditor.box.clear();

	WaresModel & wm = waresModel(dbname);
	int i = wm.index(lastWareName);
	if(i == -1)
		ware = Ware();
	else
		ware = wm.ware(i);

	quantityEditor.setSuffix(ware.unit);
	tagsWidget.setTags(ware.tags);

	QString cats = WaresModel::typesToString(ware.types);
	typeEditor.box.addItem(lastCat);
	typeEditor.box.addItems(cats.split(", ", QString::SkipEmptyParts));
	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));
}

/* We need this for cases when keybaord focus was not used,
 * just a mouse selection from the drop down popup. */
void EditItemView::wareNameEditFinishedSlot(int)
{
	wareNameEditFinishedSlot();
}
