/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <float.h>
#include <math.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_editpaymentview.h"

@include@ views.cpp
@declare@ Item

SCC TidContext = "EditPaymentView";

SCC TidNewItemWindowTitle = QT_TRANSLATE_NOOP("EditPaymentView", "Already bought new item");
SCC TidEditItemWindowTitle = QT_TRANSLATE_NOOP("EditPaymentView", "Editing an existing item");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditPaymentView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditPaymentView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditPaymentView", "Previous item");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditPaymentView", "Next item");

SCC TidTypeEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Type of ware:");
SCC TidBrandSelector = QT_TRANSLATE_NOOP("EditPaymentView", "Brand of ware:");
SCC TidQuantityEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Quantity:");
SCC TidUnitPriceEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Unit price:");
SCC TidGrossPriceEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Gross price:");
SCC TidPartnerSelector = QT_TRANSLATE_NOOP("EditPaymentView", "Business partner:");
SCC TidAccountSelector = QT_TRANSLATE_NOOP("EditPaymentView", "Account (bank):");
SCC TidInventorySelector = QT_TRANSLATE_NOOP("EditPaymentView", "Inventory:");
SCC TidWareSelector = QT_TRANSLATE_NOOP("EditPaymentView", "Common ware name:");
SCC TidPurchaseDateTimeEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Date of purchase:");
SCC TidUploadDateTimeEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Date of upload:");
SCC TidCommentEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Comments:");
SCC TidWareTags = QT_TRANSLATE_NOOP("EditPaymentView", "Tags for <i>%1</i> :");

SCC TidMandatoryField = QT_TRANSLATE_NOOP("EditPaymentView", "mandatory field");
SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditPaymentView", "Please fill at least the mandatory fields.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditPaymentView", "Item is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditPaymentView", "Item is updated.");

EditPaymentView * EditPaymentView::newItemViewFactory(const csjp::String & dbname)
{
	csjp::Object<ItemModel> ownModel = itemModel(dbname);
	EditPaymentView * view = new EditPaymentView(dbname, *ownModel);
	view->ownModel = ownModel.release();
	return view;
}

EditPaymentView::EditPaymentView(const csjp::String & dbname, ItemModel & model, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(model),
	ownModel(NULL),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right)),
	wareEditor(&wareModel(dbname), Ware::Name),
	brandEditor(&brandModel(dbname), Brand::Name),
	accountEditor(&accountModel(dbname), Account::Name),
	partnerEditor(&partnerModel(dbname), Partner::Name),
	inventoryEditor(&inventoryModel(dbname), Inventory::Name),
	tagsWidget(dbname),
	lastNumEdited(0),
	lastLastNumEdited(0)
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);
	
	item.invChangeDate = QDateTime::currentDateTime();
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
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextSlot()));
	
	connect(&wareEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&brandEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&typeEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&quantityEditor, SIGNAL(valueChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&accountEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&partnerEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&inventoryEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&invChangeDateTime.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
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

	connect(&accountEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(accountNameEditFinishedSlot()));
	connect(&accountEditor.box, SIGNAL(currentIndexChanged(int)),
			this, SLOT(accountNameEditFinishedSlot(int)));

	setupView();
	retranslate();
	loadState();
}

EditPaymentView::~EditPaymentView()
{
	if(ownModel)
		delete ownModel;
}

void EditPaymentView::showEvent(QShowEvent *event)
{
	lastWareName.clear();
	lastAccountName.clear();
	mapToGui();
	if(!cursor.isValid())
		uploadDateTime.edit.setDateTime(QDateTime::currentDateTime());

	PannView::showEvent(event);
	grossPriceEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

@include@ closeEvent loadState saveState changeEvent resizeEvent

void EditPaymentView::mapToGui()
{
	if(cursor.isValid()){
		item = Item(model.data(cursor.row()));
		accountEditor.setText(item.partner);
		accountNameEditFinishedSlot();
		partnerEditor.setText(item.partner);
		inventoryEditor.setText(item.partner);
	}

	uploadDateTime.edit.setDateTime(item.uploadDate);

	wareEditor.setText(item.name);
	wareNameEditFinishedSlot();

	typeEditor.setText(item.type);
	brandEditor.setText(item.brand);

	quantityEditor.blockSignals(true);
	quantityEditor.setValue(item.quantity);
	quantityEditor.blockSignals(false);

	commentEditor.edit.setText(item.comment);

	invChangeDateTime.edit.setDateTime(item.invChangeDate);

	unitPriceEditor.blockSignals(true);
	unitPriceEditor.setValue((0.001 <= item.quantity) ? item.price / item.quantity : 0);
	unitPriceEditor.blockSignals(false);

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.setValue(item.price);
	grossPriceEditor.blockSignals(false);

	updateToolButtonStates();
}

void EditPaymentView::mapFromGui()
{
	item.uploadDate = uploadDateTime.edit.dateTime();

	item.account = accountEditor.text();
	item.partner = partnerEditor.text();
	item.inventory = inventoryEditor.text();
	item.name = wareEditor.text();
	item.unit <<= quantityEditor.getSuffix();
	item.type = typeEditor.text();
	item.brand = brandEditor.text();
	item.quantity = quantityEditor.value();
	item.comment = commentEditor.edit.toPlainText();

	item.price = grossPriceEditor.value();
	item.currency <<= grossPriceEditor.getSuffix();
	item.invChangeDate = invChangeDateTime.edit.dateTime();

	ware.name = item.name;
	ware.setAsTags(tagsWidget.selectedTags());
	if(typeEditor.text().size() && !ware.types.has(item.type))
		ware.types.add(new WareType(ware.name, typeEditor.text()));
}

void EditPaymentView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditItemWindowTitle));
	else
		setWindowTitle(tr(TidNewItemWindowTitle));

	wareEditor.label.setText(tr(TidWareSelector));
	wareEditor.editor.setPlaceholderText(tr(TidMandatoryField));
	typeEditor.label.setText(tr(TidTypeEditor));
	brandEditor.label.setText(tr(TidBrandSelector));
	quantityEditor.label.setText(tr(TidQuantityEditor));
	unitPriceEditor.label.setText(tr(TidUnitPriceEditor));
	grossPriceEditor.label.setText(tr(TidGrossPriceEditor));
	accountEditor.label.setText(tr(TidAccountSelector));
	partnerEditor.label.setText(tr(TidPartnerSelector));
	inventoryEditor.label.setText(tr(TidInventorySelector));
	invChangeDateTime.label.setText(tr(TidPurchaseDateTimeEditor));
	uploadDateTime.label.setText(tr(TidUploadDateTimeEditor));
	commentEditor.label.setText(tr(TidCommentEditor));

	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));

	relayout();
}

void EditPaymentView::applyLayout(bool test)
{
	HLayout * hlayout = new HLayout;
	hlayout->addWidget(&grossPriceEditor);
	hlayout->addStretch();
	hlayout->addWidget(&quantityEditor);
	hlayout->addStretch();
	hlayout->addWidget(&unitPriceEditor);

	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addLayout(hlayout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&wareEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&typeEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&brandEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&accountEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&partnerEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&inventoryEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&invChangeDateTime);
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

void EditPaymentView::relayout()
{
	{
		wareEditor.wideLayout();
		typeEditor.wideLayout();
		brandEditor.wideLayout();
		quantityEditor.wideLayout();
		unitPriceEditor.wideLayout();
		grossPriceEditor.wideLayout();
		accountEditor.wideLayout();
		partnerEditor.wideLayout();
		inventoryEditor.wideLayout();
		invChangeDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout( true);
	}
	if(width() < sizeHint().width()){
		wareEditor.wideLayout();
		typeEditor.wideLayout();
		brandEditor.wideLayout();
		quantityEditor.narrowLayout();
		unitPriceEditor.narrowLayout();
		grossPriceEditor.narrowLayout();
		accountEditor.wideLayout();
		partnerEditor.wideLayout();
		inventoryEditor.wideLayout();
		invChangeDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout(true);
	}
	if(width() < sizeHint().width()){
		wareEditor.narrowLayout();
		typeEditor.narrowLayout();
		brandEditor.narrowLayout();
		quantityEditor.narrowLayout();
		unitPriceEditor.narrowLayout();
		grossPriceEditor.narrowLayout();
		accountEditor.narrowLayout();
		partnerEditor.narrowLayout();
		inventoryEditor.narrowLayout();
		invChangeDateTime.narrowLayout();
		uploadDateTime.narrowLayout();
		commentEditor.narrowLayout();
		applyLayout(true);
	}

	applyLayout();
	updateToolButtonStates();
}

void EditPaymentView::updateToolButtonStates()
{
	bool modified = !(
			item.account == accountEditor.text() &&
			item.partner == partnerEditor.text() &&
			item.inventory == inventoryEditor.text() &&
			item.name == wareEditor.text() &&
			item.type == typeEditor.text() &&
			item.brand == brandEditor.text() &&
			(item.quantity - quantityEditor.value()).abs() < 0.001 &&
			item.comment == commentEditor.edit.toPlainText() &&
			(item.price - grossPriceEditor.value()).abs() < 0.01 &&
			item.invChangeDate == invChangeDateTime.edit.dateTime()
			);

	/* tag states might have changed for ware */
	if(ware.tags != tagsWidget.selectedTags())
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

void EditPaymentView::saveSlotSpec()
{
	int i;

	/* Add partner if not yet known. */
	PartnerModel & sm = partnerModel(dbname);
	i = sm.index(partnerEditor.text());
	if(i == -1){
		Partner partner;
		partner.name = partnerEditor.text();
		partner.company = partnerEditor.text();
		/* Add company if not yet known. */
		CompanyModel & cm = companyModel(dbname);
		int i = cm.index(partner.company);
		if(i == -1){
			Company company;
			company.name = partner.company;
			cm.addNew(company);
		}
		sm.addNew(partner);
	}

	/* Add brand if not yet known. */
	BrandModel & bm = brandModel(dbname);
	i = bm.index(brandEditor.text());
	if(i == -1){
		Brand brand;
		brand.name = brandEditor.text();
		brand.company = partnerEditor.text();
		/* Add company if not yet known. */
		CompanyModel & cm = companyModel(dbname);
		int i = cm.index(brand.company);
		if(i == -1){
			Company company;
			company.name = brand.company;
			cm.addNew(company);
		}
		bm.addNew(brand);
	}

	/* Add account if not yet known. */
	AccountModel & am = accountModel(dbname);
	i = am.index(accountEditor.text());
	if(i == -1){
		Account account;
		account.name = accountEditor.text();
		am.addNew(account);
	}

	/* Add inventory if not yet known. */
	InventoryModel & im = inventoryModel(dbname);
	i = im.index(inventoryEditor.text());
	if(i == -1){
		Inventory inventory;
		inventory.name = inventoryEditor.text();
		im.addNew(inventory);
	}

	/* Add/update ware if neccessary. */
	WareModel & wm = wareModel(dbname);
	i = wm.index(ware.name);
	if(i == -1)
		wm.addNew(ware);
	else if(wm.data(i) != ware)
		wm.update(i, ware);
}

@include@ setCursor prevSlot nextSlot saveSlot

void EditPaymentView::resetSlot()
{
	mapToGui();
	tagsWidget.setTags(ware.tags);
}

void EditPaymentView::quantityValueChangedSlot(double q)
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

void EditPaymentView::unitPriceValueChangedSlot(double u)
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

void EditPaymentView::grossPriceValueChangedSlot(double g)
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

void EditPaymentView::wareNameEditFinishedSlot()
{
	if(lastWareName == wareEditor.editor.text())
		return;

	lastWareName = wareEditor.editor.text();

	QString lastCat = typeEditor.text();
	typeEditor.box.clear();

	WareModel & wm = wareModel(dbname);
	int i = wm.index(lastWareName);
	if(i == -1)
		ware = Ware();
	else
		ware = wm.data(i);

	quantityEditor.setSuffix(ware.unit);
	tagsWidget.setTags(ware.tags);

	QStringList cats;
	cats <<= ware.types;
	typeEditor.box.addItem(lastCat);
	typeEditor.box.addItems(cats);
	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));
}

/* We need this for cases when keyboard focus was not used,
 * just a mouse selection from the drop down popup. */
void EditPaymentView::wareNameEditFinishedSlot(int)
{
	wareNameEditFinishedSlot();
}

void EditPaymentView::accountNameEditFinishedSlot()
{
	if(lastAccountName == accountEditor.editor.text())
		return;

	lastAccountName = accountEditor.editor.text();

	Account account;
	AccountModel & am = accountModel(dbname);
	int i = am.index(lastAccountName);
	if(i != -1)
		account = am.data(i);

	grossPriceEditor.setSuffix(account.currency);
}

/* We need this for cases when keyboard focus was not used,
 * just a mouse selection from the drop down popup. */
void EditPaymentView::accountNameEditFinishedSlot(int)
{
	accountNameEditFinishedSlot();
}
