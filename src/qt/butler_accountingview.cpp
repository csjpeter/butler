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
	shopEditor(&shopsModel(dbname), Shop::Name),
	wareEditor(&waresModel(dbname), Ware::Name),
	lastSpinEdited(0),
	lastLastSpinEdited(0)
{
	setWindowModality(Qt::ApplicationModal);
	
	boughtCheck.box.setCheckState(Qt::Checked);
	uploadDateTime.setEnabled(false);

	infoLabel.setProperty("infoField", true);
	infoLabel.setAlignment(Qt::AlignCenter);

	doneButton.setDefault(true);

	connect(&backButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	
	connect(&shopEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&wareEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&grossPriceEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&quantityEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));

	connect(&quantityEditor, SIGNAL(valueChanged(double)),
			this, SLOT(quantityValueChangedSlot(double)));
	connect(&unitPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(unitPriceValueChangedSlot(double)));
	connect(&grossPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(grossPriceValueChangedSlot(double)));

	connect(&quantityEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(quantityEditingFinishedSlot()));
	connect(&unitPriceEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(unitPriceEditingFinishedSlot()));
	connect(&grossPriceEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(grossPriceEditingFinishedSlot()));

	connect(&wareEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(wareNameEditFinishedSlot()));
	connect(&wareEditor.box, SIGNAL(currentIndexChanged(int)),
			this, SLOT(wareNameEditFinishedSlot(int)));
	connect(&quantityEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(quantityEditFinishedSlot()));

	/* restore last state */
	loadState();

	retranslate();
}

void AccountingView::showEvent(QShowEvent *event)
{
	uploadDateTime.edit.setDateTime(QDateTime::currentDateTime());

	PannView::showEvent(event);

	mapToGui();
}

void AccountingView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void AccountingView::loadState()
{
	QSettings settings;
	QPoint pos = settings.value("accountingview/position", QPoint()).toPoint();
	QSize size = settings.value("accountingview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void AccountingView::saveState()
{
	QSettings settings;
	settings.setValue("accountingview/position", pos());
	settings.setValue("accountingview/size", size());
}

void AccountingView::mapToGui()
{
	uploadDateTime.edit.setDateTime(item.uploaded);

	wareEditor.setText(item.name);
	wareNameEditFinishedSlot();
	categoryEditor.setText(item.category);

	quantityEditor.editor.blockSignals(true);
	quantityEditor.setValue(item.quantity);
	quantityEditor.editor.blockSignals(false);

	commentEditor.edit.setText(item.comment);

	unitPriceEditor.editor.blockSignals(true);
	unitPriceEditor.setValue((0.001 <= item.quantity) ? item.price / item.quantity : 0);
	unitPriceEditor.editor.blockSignals(false);

	grossPriceEditor.editor.blockSignals(true);
	grossPriceEditor.setValue(item.price);
	grossPriceEditor.editor.blockSignals(false);

	onStockCheck.box.setCheckState(item.onStock ? Qt::Checked : Qt::Unchecked);
}

void AccountingView::mapFromGui()
{
	item.uploaded = uploadDateTime.edit.dateTime();
/*
	int i = shopEditor.box.currentIndex();
	ShopsModel &sm = shopsModel(dbname);
	if(0 <= i && i < sm.rowCount())
		item.shop = sm.shop(i).name;
*/
	item.shop = shopEditor.text();
	item.name = wareEditor.text();
	item.category = categoryEditor.text();
	item.quantity = quantityEditor.value();
	item.comment = commentEditor.edit.toPlainText();

	item.bought = (boughtCheck.box.checkState() == Qt::Checked);
	item.price = grossPriceEditor.value();
	item.purchased = purchaseDateTime.edit.dateTime();

	item.onStock = (onStockCheck.box.checkState() == Qt::Checked);
}

void AccountingView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void AccountingView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize() || !isVisible()))
		return;
	relayout();
}

void AccountingView::retranslate()
{
	setWindowTitle(qtTrId(TidAccountingWindowTitle));
	backButton.setText(qtTrId(TidBackButtonLabel));
	doneButton.setText(qtTrId(TidDoneButtonLabel));
	shopEditor.label.setText(qtTrId(TidShopSelectorLabel));
	shopEditor.editor.setPlaceholderText(qtTrId(TidShopSelectorPlaceholder));
	wareEditor.label.setText(qtTrId(TidWareSelectorLabel));
	wareEditor.editor.setPlaceholderText(qtTrId(TidWareSelectorPlaceholder));
	categoryEditor.label.setText(qtTrId(TidCategoryEditorLabel));
	quantityEditor.label.setText(qtTrId(TidQuantityEditorLabel));
	quantityEditor.editor.setPlaceholderText(qtTrId(TidQuantityEditorPlaceholder));
	unitPriceEditor.label.setText(qtTrId(TidUnitPriceEditorLabel));
	grossPriceEditor.label.setText(qtTrId(TidGrossPriceEditorLabel));
	purchaseDateTime.label.setText(qtTrId(TidPurchaseDateTimeEditorLabel));
	uploadDateTime.label.setText(qtTrId(TidUploadDateTimeEditorLabel));
	commentEditor.label.setText(qtTrId(TidCommentEditorLabel));
	onStockCheck.label.setText(qtTrId(TidOnStockFormCheckBoxLabel));
	boughtCheck.label.setText(qtTrId(TidBoughtFormCheckBoxLabel));

	relayout();
}

enum class ViewState {
	Wide,
	Medium,
	Narrow
};

void AccountingView::applyLayout()
{
	delete layout();

	HLayout * toolLayout = new HLayout;
	toolLayout->addWidget(&infoLabel, 1);
	toolLayout->addStretch(0);
	toolLayout->addWidget(&backButton);
	toolLayout->addWidget(&doneButton);

	HLayout * hlayout = new HLayout;
	hlayout->addWidget(&quantityEditor);
	hlayout->addStretch();
	hlayout->addWidget(&grossPriceEditor);
	hlayout->addStretch();
	hlayout->addWidget(&unitPriceEditor);

	HLayout * h2layout = new HLayout;
	h2layout->addWidget(&onStockCheck);
	h2layout->addStretch();
	h2layout->addWidget(&boughtCheck);
	h2layout->addStretch();

	VLayout * mainLayout = new VLayout;
	mainLayout->addLayout(toolLayout);
	mainLayout->addSpacing(3);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&shopEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&wareEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&categoryEditor);
	mainLayout->addStretch(0);
	mainLayout->addLayout(hlayout);
	mainLayout->addStretch(0);
	mainLayout->addLayout(h2layout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&purchaseDateTime);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&uploadDateTime);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&commentEditor);

	setLayout(mainLayout);
	mandatoryFieldChangedSlot("");
}

void AccountingView::relayout()
{
	ViewState newState = ViewState::Wide;
	QSize newSize;

	switch(newState) {
		case ViewState::Wide :
			shopEditor.wideLayout();
			wareEditor.wideLayout();
			categoryEditor.wideLayout();
			quantityEditor.wideLayout();
			unitPriceEditor.wideLayout();
			grossPriceEditor.wideLayout();
			purchaseDateTime.wideLayout();
			uploadDateTime.wideLayout();
			commentEditor.wideLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Medium :
			shopEditor.wideLayout();
			wareEditor.wideLayout();
			categoryEditor.wideLayout();
			quantityEditor.narrowLayout();
			unitPriceEditor.narrowLayout();
			grossPriceEditor.narrowLayout();
			purchaseDateTime.wideLayout();
			uploadDateTime.wideLayout();
			commentEditor.wideLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Narrow :
			shopEditor.narrowLayout();
			wareEditor.narrowLayout();
			categoryEditor.narrowLayout();
			quantityEditor.narrowLayout();
			unitPriceEditor.narrowLayout();
			grossPriceEditor.narrowLayout();
			purchaseDateTime.narrowLayout();
			uploadDateTime.narrowLayout();
			commentEditor.narrowLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
			break;
	}
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

	/* Add ware if not yet known. */
	WaresModel &wm = waresModel(dbname);
	i = wm.index(wareEditor.text());
	if(i == -1){
		Ware ware;
		ware.name = wareEditor.text();
		if(categoryEditor.text().size())
			ware.categories.add(new QString(categoryEditor.text()));
		wm.addNew(ware);
	} else if(!wm.ware(i).categories.has(categoryEditor.text())) {
		Ware modified(wm.ware(i));
		modified.categories.add(new QString(categoryEditor.text()));
		wm.update(i, modified);
	}

	model.addNew(item);

	item = Item();
	item.uploaded = QDateTime::currentDateTime();
	mapToGui();
	wareEditor.editor.setFocus(Qt::OtherFocusReason);

	infoLabel.setText(qtTrId(TidAccountingSavedInfoLabel));
	infoLabel.updateGeometry();
}

void AccountingView::quantityValueChangedSlot(double q)
{
	double u = unitPriceEditor.value();
	double g = grossPriceEditor.value();

	if((lastSpinEdited == &quantityEditor && lastLastSpinEdited == &grossPriceEditor) ||
			lastSpinEdited == &grossPriceEditor){
		u = (q < 0.001) ? 0 : g / q;
		unitPriceEditor.editor.blockSignals(true);
		unitPriceEditor.setValue(u);
		unitPriceEditor.editor.blockSignals(false);
		return;
	}

	grossPriceEditor.editor.blockSignals(true);
	grossPriceEditor.setValue(u * q);
	grossPriceEditor.editor.blockSignals(false);
}

void AccountingView::unitPriceValueChangedSlot(double u)
{
	double q = quantityEditor.value();
	double g = grossPriceEditor.value();

	if((lastSpinEdited == &unitPriceEditor && lastLastSpinEdited == &grossPriceEditor) ||
			lastSpinEdited == &grossPriceEditor){
		q = (u < 0.01) ? 0 : g / u;
		quantityEditor.editor.blockSignals(true);
		quantityEditor.setValue(q);
		quantityEditor.editor.blockSignals(false);
		return;
	}

	grossPriceEditor.editor.blockSignals(true);
	grossPriceEditor.setValue(u * q);
	grossPriceEditor.editor.blockSignals(false);
}

void AccountingView::grossPriceValueChangedSlot(double g)
{
	double q = quantityEditor.value();
	double u = unitPriceEditor.value();

	if((lastSpinEdited == &grossPriceEditor && lastLastSpinEdited == &unitPriceEditor) ||
			lastSpinEdited == &unitPriceEditor){
		q = (u < 0.01) ? 0 : g / u;
		quantityEditor.editor.blockSignals(true);
		quantityEditor.setValue(q);
		quantityEditor.editor.blockSignals(false);
		return;
	}

	u = (q < 0.001) ? 0 : g / q;
	unitPriceEditor.editor.blockSignals(true);
	unitPriceEditor.setValue(u);
	unitPriceEditor.editor.blockSignals(false);
}

void AccountingView::quantityEditingFinishedSlot()
{
	if(lastSpinEdited != &quantityEditor)
		lastLastSpinEdited = lastSpinEdited;
	lastSpinEdited = &quantityEditor;
}

void AccountingView::unitPriceEditingFinishedSlot()
{
	if(lastSpinEdited != &unitPriceEditor)
		lastLastSpinEdited = lastSpinEdited;
	lastSpinEdited = &unitPriceEditor;
}

void AccountingView::grossPriceEditingFinishedSlot()
{
	if(lastSpinEdited != &grossPriceEditor)
		lastLastSpinEdited = lastSpinEdited;
	lastSpinEdited = &grossPriceEditor;
}

void AccountingView::mandatoryFieldChangedSlot(const QString &)
{
	if(	shopEditor.editor.text().size() &&
		wareEditor.editor.text().size() &&
		quantityEditor.value() != 0
	) {
		DBG("Lets show button");
		if(!doneButton.isVisible())
			doneButton.show();
		if(infoLabel.text().size()){
			infoLabel.setText("");
			infoLabel.updateGeometry();
		}
		return;
	}

	doneButton.hide();
	if(infoLabel.text() != qtTrId(TidFillMandatoryFieldsInfoLabel)){
		infoLabel.setText(qtTrId(TidFillMandatoryFieldsInfoLabel));
		infoLabel.updateGeometry();
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
	if(i == -1){
		quantityEditor.setSuffix();
		return;
	}

	const Ware &w = wm.ware(i);

	QString cats = WaresModel::categoriesToString(w.categories);
	categoryEditor.box.addItem(lastCat);
	categoryEditor.box.addItems(cats.split(", ", QString::SkipEmptyParts));

	quantityEditor.setSuffix(w.unit);
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
