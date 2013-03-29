/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

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
	shopSelector(dbname),
	wareEditor(dbname)
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Add already bought new item"));
	
	boughtCheck.setCheckState(Qt::Checked);

	relayout();

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	
	connect(&wareEditor.lineEdit, SIGNAL(editingFinished()),
			this, SLOT(nameEditFinishedSlot()));
	connect(&quantityEditor, SIGNAL(valueChanged(double)),
			this, SLOT(quantityValueChangedSlot(double)));
	connect(&unitPriceEditor, SIGNAL(editingFinished()),
			this, SLOT(unitPriceEditingFinishedSlot()));
	connect(&grossPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(grossPriceValueChangedSlot(double)));

	/* restore last state */
	loadState();
}

void AccountingView::relayout()
{
	QHBoxLayout * toolLayout = new QHBoxLayout;
	toolLayout->addStretch();
	toolLayout->addWidget(&doneButton);

	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->addLayout(quantityEditor.portrait());
	hlayout->addLayout(grossPriceEditor.portrait());
	hlayout->addLayout(unitPriceEditor.portrait());

	QHBoxLayout * h2layout = new QHBoxLayout;
	h2layout->addWidget(&onStockCheck);
	h2layout->addWidget(&boughtCheck);

	QVBoxLayout * layout = new QVBoxLayout;

	if(orientation == ScreenOrientation::Portrait) {
		layout->addLayout(toolLayout);
		layout->addSpacing(3);
		layout->addLayout(shopSelector.landscape());
		layout->addLayout(wareEditor.landscape());
		layout->addLayout(categoryEditor.landscape());
		layout->addLayout(hlayout);
		layout->addLayout(h2layout);
		layout->addLayout(purchaseDateTime.landscape());
		layout->addLayout(commentEditor.landscape());
		layout->addLayout(uploadDateTime.landscape());
		orientation = ScreenOrientation::Landscape;
	} else {
		layout->addLayout(toolLayout);
		layout->addLayout(shopSelector.portrait());
		layout->addLayout(wareEditor.portrait());
		layout->addLayout(categoryEditor.portrait());
		layout->addLayout(hlayout);
		layout->addLayout(h2layout);
		layout->addLayout(purchaseDateTime.portrait());
		layout->addLayout(commentEditor.portrait());
		layout->addLayout(uploadDateTime.portrait());
		orientation = ScreenOrientation::Portrait;
	}

	setLayout(layout);
	shopSelector.tableView.enableKineticScroll();
	wareEditor.enableKineticScroll();
	categoryEditor.tableView.enableKineticScroll();
}

void AccountingView::showEvent(QShowEvent *event)
{
	uploadDateTime.setDateTime(QDateTime::currentDateTime());

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
	uploadDateTime.setDateTime(item.uploaded);

	wareEditor.setText(item.name);
	nameEditFinishedSlot();
	categoryEditor.setText(item.category);

	quantityEditor.blockSignals(true);
	quantityEditor.setValue(item.quantity);
	quantityEditor.blockSignals(false);

	commentEditor.setText(item.comment);

	unitPriceEditor.blockSignals(true);
	unitPriceEditor.setValue((DBL_EPSILON <= item.quantity) ?
			item.price / item.quantity : 0);
	unitPriceEditor.blockSignals(false);

	grossPriceEditor.blockSignals(true);
	grossPriceEditor.setValue(item.price);
	grossPriceEditor.blockSignals(false);

	onStockCheck.setCheckState(item.onStock ? Qt::Checked : Qt::Unchecked);
}

void AccountingView::mapFromGui()
{
	item.uploaded = uploadDateTime.dateTime();

	item.name = wareEditor.text();
	item.category = categoryEditor.text();
	item.quantity = quantityEditor.value();
	item.comment = commentEditor.toPlainText();

	item.bought = (boughtCheck.checkState() == Qt::Checked);
	item.price = grossPriceEditor.value();
	item.purchased = purchaseDateTime.dateTime();

	int i = shopSelector.currentIndex();
	ShopsModel &sm = shopsModel(dbname);
	if(0 <= i && i < sm.rowCount())
		item.shop = sm.shop(i).name;

	item.onStock = (onStockCheck.checkState() == Qt::Checked);
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
}

void AccountingView::nameEditFinishedSlot()
{
	categoryEditor.clear();

	WaresModel &wm = waresModel(dbname);
	int i = wm.index(wareEditor.text());
	if(i == -1){
		quantityEditor.setSuffix("");
		return;
	}

	const Ware &w = wm.ware(i);

	QString cats = WaresModel::categoriesToString(w.categories);
	categoryEditor.addItems(cats.split(", ", QString::SkipEmptyParts));

	quantityEditor.setSuffix(" " + w.unit);
}

void AccountingView::quantityValueChangedSlot(double q)
{
	double u = unitPriceEditor.value();
	double g = grossPriceEditor.value();

	if(-DBL_EPSILON < g && g < DBL_EPSILON){
		grossPriceEditor.blockSignals(true);
		grossPriceEditor.setValue(u * q);
		grossPriceEditor.blockSignals(false);
		return;
	}

	if(-DBL_EPSILON < q && q < DBL_EPSILON){
		u = 0;
	} else {
		u = g / q;
	}

	unitPriceEditor.blockSignals(true);
	unitPriceEditor.setValue(u);
	unitPriceEditor.blockSignals(false);
}

void AccountingView::unitPriceEditingFinishedSlot()
{
	double u = unitPriceEditor.value();
	double q = quantityEditor.value();
	double g = grossPriceEditor.value();

	if(-DBL_EPSILON < g && g < DBL_EPSILON){
		grossPriceEditor.blockSignals(true);
		grossPriceEditor.setValue(u * q);
		grossPriceEditor.blockSignals(false);
		return;
	}

	if(-DBL_EPSILON < u && u < DBL_EPSILON)
		q = 0;
	else
		q = g / u;

	quantityEditor.blockSignals(true);
	quantityEditor.setValue(q);
	quantityEditor.blockSignals(false);
}

void AccountingView::grossPriceValueChangedSlot(double g)
{
	double u = 0;

	if(-DBL_EPSILON < g && g < DBL_EPSILON){
		quantityEditor.blockSignals(true);
		quantityEditor.setValue(0);
		quantityEditor.blockSignals(false);
	}

	double q = quantityEditor.value();
	if(-DBL_EPSILON < q && q < DBL_EPSILON){
		u = 0;
	} else {
		u = g / q;
	}

	unitPriceEditor.blockSignals(true);
	unitPriceEditor.setValue(u);
	unitPriceEditor.blockSignals(false);
}
