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

SCC TidTypeEditor = QT_TRANSLATE_NOOP("EditItemView", "Type of ware:");
SCC TidBrandSelector = QT_TRANSLATE_NOOP("EditItemView", "Brand of ware:");
SCC TidQuantityEditor = QT_TRANSLATE_NOOP("EditItemView", "Quantity:");
SCC TidUnitPriceEditor = QT_TRANSLATE_NOOP("EditItemView", "Unit price:");
SCC TidGrossPriceEditor = QT_TRANSLATE_NOOP("EditItemView", "Gross price:");
SCC TidPartnerSelector = QT_TRANSLATE_NOOP("EditItemView", "Business partner:");
SCC TidAccountSelector = QT_TRANSLATE_NOOP("EditItemView", "Account (bank):");
SCC TidInventorySelector = QT_TRANSLATE_NOOP("EditItemView", "Inventory:");
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
	csjp::Object<ItemsModel> ownModel = itemModel(dbname);
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
	brandEditor(&brandsModel(dbname), Brand::Name),
	accountEditor(&accountsModel(dbname), Account::Name),
	partnerEditor(&partnersModel(dbname), Partner::Name),
	inventoryEditor(&inventoriesModel(dbname), Inventory::Name),
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
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevClickedSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextClickedSlot()));
	
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

EditItemView::~EditItemView()
{
	if(ownModel)
		delete ownModel;
}

void EditItemView::showEvent(QShowEvent *event)
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

void EditItemView::mapFromGui()
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
	//ware.tags = tagsWidget.selectedTags();
	//if(typeEditor.text().size() && !ware.types.has(item.type))
	//	ware.types.add(new Text(typeEditor.text()));
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

void EditItemView::applyLayout(bool test)
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

void EditItemView::relayout()
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

void EditItemView::updateToolButtonStates()
{
	bool modified = !(
			item.account == accountEditor.text() &&
			item.partner == partnerEditor.text() &&
			item.inventory == inventoryEditor.text() &&
			item.name == wareEditor.text() &&
			item.type == typeEditor.text() &&
			item.brand == brandEditor.text() &&
			fabs(item.quantity - quantityEditor.value()) < 0.001 &&
			item.comment == commentEditor.edit.toPlainText() &&
			fabs(item.price - grossPriceEditor.value()) < 0.01 &&
			item.invChangeDate == invChangeDateTime.edit.dateTime()
			);

	/* tag states might have changed for ware */
	//if(tagsWidget.selectedTags() != ware.tags)
	//	modified = true;

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
	int i;

	/* Add partner if not yet known. */
	PartnersModel & sm = partnersModel(dbname);
	i = sm.index(partnerEditor.text());
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

	/* Add brand if not yet known. */
	BrandsModel & bm = brandsModel(dbname);
	i = bm.index(brandEditor.text());
	if(i == -1){
		Brand brand;
		brand.name = brandEditor.text();
		brand.company = partnerEditor.text();
		/* Add company if not yet known. */
		CompaniesModel & cm = companiesModel(dbname);
		int i = cm.index(brand.company);
		if(i == -1){
			Company company;
			company.name = brand.company;
			cm.addNew(company);
		}
		bm.addNew(brand);
	}

	/* Add account if not yet known. */
	AccountsModel & am = accountsModel(dbname);
	i = am.index(accountEditor.text());
	if(i == -1){
		Account account;
		account.name = accountEditor.text();
		am.addNew(account);
	}

	/* Add inventory if not yet known. */
	InventoriesModel & im = inventoriesModel(dbname);
	i = im.index(inventoryEditor.text());
	if(i == -1){
		Inventory inventory;
		inventory.name = inventoryEditor.text();
		im.addNew(inventory);
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
		item.uploadDate = QDateTime::currentDateTime();
		ware = Ware();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		grossPriceEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void EditItemView::resetSlot()
{
	mapToGui();
	//tagsWidget.setTags(ware.tags);
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
	StringSet set;
	set <<= ware.tags;
	tagsWidget.setTags(set);

	QStringList cats;
	cats <<= ware.types;
	typeEditor.box.addItem(lastCat);
	typeEditor.box.addItems(cats);
	tagsWidget.label.setText(tr(TidWareTags).arg(Config::locale.quoteString(ware.name)));
}

/* We need this for cases when keyboard focus was not used,
 * just a mouse selection from the drop down popup. */
void EditItemView::wareNameEditFinishedSlot(int)
{
	wareNameEditFinishedSlot();
}

void EditItemView::accountNameEditFinishedSlot()
{
	if(lastAccountName == accountEditor.editor.text())
		return;

	lastAccountName = accountEditor.editor.text();

	Account account;
	AccountsModel & am = accountsModel(dbname);
	int i = am.index(lastAccountName);
	if(i != -1)
		account = am.account(i);

	grossPriceEditor.setSuffix(account.currency);
}

/* We need this for cases when keyboard focus was not used,
 * just a mouse selection from the drop down popup. */
void EditItemView::accountNameEditFinishedSlot(int)
{
	accountNameEditFinishedSlot();
}
