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
	shopSelector(&shopsModel(dbname), Shop::Name),
	wareEditor(&waresModel(dbname), Ware::Name),
	lastSpinEdited(0),
	lastLastSpinEdited(0)
{
	setWindowModality(Qt::ApplicationModal);
	
	boughtCheck.setCheckState(Qt::Checked);
	uploadDateTime.setEnabled(false);

	infoLabel.setProperty("infoField", true);
	infoLabel.setAlignment(Qt::AlignCenter);

	shopSelector.setProperty("mandatoryField", true);
	wareEditor.setProperty("mandatoryField", true);
	quantityEditor.setProperty("mandatoryField", true);

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	
	connect(&shopSelector.box, SIGNAL(currentIndexChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&wareEditor.lineEdit, SIGNAL(textChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&grossPriceEditor.spin, SIGNAL(valueChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));
	connect(&quantityEditor.spin, SIGNAL(valueChanged(const QString &)),
			this, SLOT(mandatoryFieldChangedSlot(const QString &)));

	connect(&quantityEditor.spin, SIGNAL(valueChanged(double)),
			this, SLOT(quantityValueChangedSlot(double)));
	connect(&unitPriceEditor.spin, SIGNAL(valueChanged(double)),
			this, SLOT(unitPriceValueChangedSlot(double)));
	connect(&grossPriceEditor.spin, SIGNAL(valueChanged(double)),
			this, SLOT(grossPriceValueChangedSlot(double)));

	connect(&quantityEditor.spin, SIGNAL(editingFinished()),
			this, SLOT(quantityEditingFinishedSlot()));
	connect(&unitPriceEditor.spin, SIGNAL(editingFinished()),
			this, SLOT(unitPriceEditingFinishedSlot()));
	connect(&grossPriceEditor.spin, SIGNAL(editingFinished()),
			this, SLOT(grossPriceEditingFinishedSlot()));

	connect(&wareEditor.lineEdit, SIGNAL(editingFinished()),
			this, SLOT(nameEditFinishedSlot()));

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
	nameEditFinishedSlot();
	categoryEditor.setText(item.category);

	quantityEditor.blockSignals(true);
	quantityEditor.spin.setValue(item.quantity);
	quantityEditor.spin.blockSignals(false);

	commentEditor.edit.setText(item.comment);

	unitPriceEditor.blockSignals(true);
	unitPriceEditor.spin.setValue((DBL_EPSILON <= item.quantity) ?
			item.price / item.quantity : 0);
	unitPriceEditor.blockSignals(false);

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.spin.setValue(item.price);
	grossPriceEditor.blockSignals(false);

	onStockCheck.setCheckState(item.onStock ? Qt::Checked : Qt::Unchecked);
}

void AccountingView::mapFromGui()
{
	item.uploaded = uploadDateTime.edit.dateTime();

	item.name = wareEditor.text();
	item.category = categoryEditor.text();
	item.quantity = quantityEditor.spin.value();
	item.comment = commentEditor.edit.toPlainText();

	item.bought = (boughtCheck.checkState() == Qt::Checked);
	item.price = grossPriceEditor.spin.value();
	item.purchased = purchaseDateTime.edit.dateTime();

	int i = shopSelector.box.currentIndex();
	ShopsModel &sm = shopsModel(dbname);
	if(0 <= i && i < sm.rowCount())
		item.shop = sm.shop(i).name;

	item.onStock = (onStockCheck.checkState() == Qt::Checked);
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
	doneButton.setText(qtTrId(TidDoneButtonLabel));
	shopSelector.label.setText(qtTrId(TidShopEditorLabel));
	wareEditor.label.setText(qtTrId(TidWareEditorLabel));
	categoryEditor.label.setText(qtTrId(TidCategoryEditorLabel));
	quantityEditor.label.setText(qtTrId(TidQuantityEditorLabel));
	unitPriceEditor.label.setText(qtTrId(TidUnitPriceEditorLabel));
	grossPriceEditor.label.setText(qtTrId(TidGrossPriceEditorLabel));
	purchaseDateTime.label.setText(qtTrId(TidPurchaseDateTimeEditorLabel));
	uploadDateTime.label.setText(qtTrId(TidUploadDateTimeEditorLabel));
	commentEditor.label.setText(qtTrId(TidCommentEditorLabel));
	onStockCheck.setText(qtTrId(TidOnStockCheckBoxLabel));
	boughtCheck.setText(qtTrId(TidBoughtCheckBoxLabel));

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

	HLayout * hlayout = new HLayout;
	hlayout->addWidget(&quantityEditor);
	hlayout->addStretch();
	hlayout->addWidget(&grossPriceEditor);
	hlayout->addStretch();
	hlayout->addWidget(&unitPriceEditor);

	HLayout * h2layout = new HLayout;
	h2layout->addStretch();
	h2layout->addWidget(&onStockCheck);
	h2layout->addWidget(&boughtCheck);

	VLayout * mainLayout = new VLayout;
	mainLayout->addLayout(toolLayout);
	mainLayout->addSpacing(3);
	mainLayout->addWidget(&shopSelector);
	mainLayout->addWidget(&wareEditor);
	mainLayout->addWidget(&categoryEditor);
	mainLayout->addLayout(hlayout);
	mainLayout->addLayout(h2layout);
	mainLayout->addWidget(&purchaseDateTime);
	mainLayout->addWidget(&uploadDateTime);
	mainLayout->addWidget(&commentEditor);

	setLayout(mainLayout);

	shopSelector.tableView.enableKineticScroll();
	wareEditor.enableKineticScroll();
	categoryEditor.tableView.enableKineticScroll();
}

void AccountingView::relayout()
{
	ViewState newState = ViewState::Wide;
	QSize newSize;

	switch(newState) {
		case ViewState::Wide :
			shopSelector.wideLayout();
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
			shopSelector.wideLayout();
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
			shopSelector.narrowLayout();
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

	model.addNew(item);

	/* We want to save any new ware and category before closing dialog. */
	WaresModel &wm = waresModel(dbname);
	int i = wm.index(wareEditor.text());
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

	item = Item();
	item.uploaded = QDateTime::currentDateTime();
	mapToGui();
	wareEditor.lineEdit.setFocus(Qt::OtherFocusReason);

	infoLabel.setText(qtTrId(TidAccountingSavedInfoLabel));
	infoLabel.updateGeometry();
}

void AccountingView::quantityValueChangedSlot(double q)
{
	double u = unitPriceEditor.spin.value();
	double g = grossPriceEditor.spin.value();

	if((lastSpinEdited == &quantityEditor && lastLastSpinEdited == &grossPriceEditor) ||
			lastSpinEdited == &grossPriceEditor){
		u = (q < 0.001) ? 0 : g / q;
		unitPriceEditor.blockSignals(true);
		unitPriceEditor.spin.setValue(u);
		unitPriceEditor.blockSignals(false);
		return;
	}

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.spin.setValue(u * q);
	grossPriceEditor.blockSignals(false);
}

void AccountingView::unitPriceValueChangedSlot(double u)
{
	double q = quantityEditor.spin.value();
	double g = grossPriceEditor.spin.value();

	if((lastSpinEdited == &unitPriceEditor && lastLastSpinEdited == &grossPriceEditor) ||
			lastSpinEdited == &grossPriceEditor){
		q = (u < 0.01) ? 0 : g / u;
		quantityEditor.blockSignals(true);
		quantityEditor.spin.setValue(q);
		quantityEditor.blockSignals(false);
		return;
	}

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.spin.setValue(u * q);
	grossPriceEditor.blockSignals(false);
}

void AccountingView::grossPriceValueChangedSlot(double g)
{
	double q = quantityEditor.spin.value();
	double u = unitPriceEditor.spin.value();

	if((lastSpinEdited == &grossPriceEditor && lastLastSpinEdited == &unitPriceEditor) ||
			lastSpinEdited == &unitPriceEditor){
		q = (u < 0.01) ? 0 : g / u;
		quantityEditor.blockSignals(true);
		quantityEditor.spin.setValue(q);
		quantityEditor.blockSignals(false);
		return;
	}

	u = (q < 0.001) ? 0 : g / q;
	unitPriceEditor.blockSignals(true);
	unitPriceEditor.spin.setValue(u);
	unitPriceEditor.blockSignals(false);
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
	if(	shopSelector.box.currentIndex() != -1 &&
		wareEditor.box.currentText().size() &&
		quantityEditor.spin.value() != 0
	) {
		if(!doneButton.isVisible())
			doneButton.show();
		if(infoLabel.text().size()){
			infoLabel.setText("");
			infoLabel.updateGeometry();
		}
	} else {
		if(doneButton.isVisible())
			doneButton.hide();
		if(infoLabel.text() != qtTrId(TidFillMandatoryFieldsInfoLabel)){
			infoLabel.setText(qtTrId(TidFillMandatoryFieldsInfoLabel));
			infoLabel.updateGeometry();
		}
	}
}

void AccountingView::nameEditFinishedSlot()
{
	categoryEditor.box.clear();

	WaresModel &wm = waresModel(dbname);
	int i = wm.index(wareEditor.text());
	if(i == -1){
		quantityEditor.spin.setSuffix("");
		return;
	}

	const Ware &w = wm.ware(i);

	QString cats = WaresModel::categoriesToString(w.categories);
	categoryEditor.box.addItems(cats.split(", ", QString::SkipEmptyParts));

	quantityEditor.spin.setSuffix(" " + w.unit);
}
