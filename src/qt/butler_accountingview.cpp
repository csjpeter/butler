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
	lastNumEdited(0),
	lastLastNumEdited(0)
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);
	
	boughtCheck.box.setCheckState(Qt::Checked);
	uploadDateTime.setEnabled(false);

	infoLabel.setProperty("infoField", true);
	infoLabel.setAlignment(Qt::AlignCenter);

	connect(&backButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	
	connect(&wareEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&quantityEditor, SIGNAL(valueChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));

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

	/* restore last state */
	loadState();

	retranslate();
}

void AccountingView::showEvent(QShowEvent *event)
{
	uploadDateTime.edit.setDateTime(QDateTime::currentDateTime());

	PannView::showEvent(event);

	mapToGui();

	wareEditor.editor.setFocus(Qt::OtherFocusReason);
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
	if(cursor.isValid()){
		item = Item(model.item(cursor.row()));
		purchaseDateTime.setDateTime(item.purchased);
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
	toolLayout->addWidget(&doneButton);
	toolLayout->addWidget(&backButton);

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
	mainLayout->addWidget(&wareEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&categoryEditor);
	mainLayout->addStretch(0);
	mainLayout->addLayout(hlayout);
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

	setLayout(mainLayout);
	mandatoryFieldChangedSlot("");
}

void AccountingView::relayout()
{
	ViewState newState = ViewState::Wide;
	QSize newSize;

	switch(newState) {
		case ViewState::Wide :
			wareEditor.wideLayout();
			categoryEditor.wideLayout();
			quantityEditor.wideLayout();
			unitPriceEditor.wideLayout();
			grossPriceEditor.wideLayout();
			shopEditor.wideLayout();
			purchaseDateTime.wideLayout();
			uploadDateTime.wideLayout();
			commentEditor.wideLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Medium :
			wareEditor.wideLayout();
			categoryEditor.wideLayout();
			quantityEditor.narrowLayout();
			unitPriceEditor.narrowLayout();
			grossPriceEditor.narrowLayout();
			shopEditor.wideLayout();
			purchaseDateTime.wideLayout();
			uploadDateTime.wideLayout();
			commentEditor.wideLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Narrow :
			wareEditor.narrowLayout();
			categoryEditor.narrowLayout();
			quantityEditor.narrowLayout();
			unitPriceEditor.narrowLayout();
			grossPriceEditor.narrowLayout();
			shopEditor.narrowLayout();
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

void AccountingView::setCursor(const QModelIndex& index)
{
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	mapToGui();
	prevButton->setEnabled(cursor.row() > 0);
	nextButton->setEnabled(cursor.row() < model.rowCount() - 1);
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

	if(cursor.isValid()){
		model.update(cursor.row(), item);
	} else {
		model.addNew(item);

		item = Item();
		item.uploaded = QDateTime::currentDateTime();
		mapToGui();
		wareEditor.editor.setFocus(Qt::OtherFocusReason);

		infoLabel.setText(qtTrId(TidAccountingSavedInfoLabel));
		infoLabel.updateGeometry();
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
		return;
	}

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.setValue(u * q);
	grossPriceEditor.blockSignals(false);
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
		return;
	}

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.setValue(u * q);
	grossPriceEditor.blockSignals(false);
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
		return;
	}

	u = (q < 0.001) ? 0 : g / q;
	unitPriceEditor.blockSignals(true);
	unitPriceEditor.setValue(u);
	unitPriceEditor.blockSignals(false);
}

void AccountingView::mandatoryFieldChangedSlot(const QString &)
{
	if(	wareEditor.editor.text().size() &&
		0.001 <= quantityEditor.value()
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
