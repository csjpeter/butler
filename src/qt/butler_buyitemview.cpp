/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include "butler_buyitemview.h"
#include "butler_itemsmodel.h"
#include "butler_waresmodel.h"
#include "butler_partnersmodel.h"

BuyItemView::BuyItemView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(shoppingModel(dbname))
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Buy item"));

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setColumnStretch(1, 1);

	QLabel *label = NULL;

	label = new QLabel(tr("Name:"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	nameLabel = new QLabel;
	gridLayout->addWidget(nameLabel, 1, 1, 1, 3);

	label = new QLabel(tr("Quantity:"));
	gridLayout->addWidget(label, 3, 0, 1, 1);
	quantityEditor = new QDoubleSpinBox;
	quantityEditor->setRange(0, INT_MAX);
	quantityEditor->setDecimals(3);
	quantityEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(quantityEditor, 3, 1, 1, 3);

	label = new QLabel(tr("Unit price:"));
	gridLayout->addWidget(label, 4, 0, 1, 1);
	unitPriceEditor = new QDoubleSpinBox;
	unitPriceEditor->setRange(0, INT_MAX);
	unitPriceEditor->setDecimals(2);
	unitPriceEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(unitPriceEditor, 4, 1, 1, 3);

	label = new QLabel(tr("Gross price:"));
	gridLayout->addWidget(label, 5, 0, 1, 1);
	grossPriceEditor = new QDoubleSpinBox;
	grossPriceEditor->setRange(0, INT_MAX);
	grossPriceEditor->setDecimals(2);
	grossPriceEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(grossPriceEditor, 5, 1, 1, 3);

	label = new QLabel(tr("Date of purchase:"));
	gridLayout->addWidget(label, 7, 0, 1, 1);
	invChangeDateTime = new QDateTimeEdit;
	invChangeDateTime->setCalendarPopup(true);
	invChangeDateTime->setDisplayFormat(Config::dateTimeFormat());
	gridLayout->addWidget(invChangeDateTime, 7, 1, 1, 3);

	label = new QLabel(tr("Partner:"));
	gridLayout->addWidget(label, 8, 0, 1, 1);
	partnerBox = new QComboBox;
	partnerBox->setModel(&partnersModel(dbname));
	partnerBox->setModelColumn(Partner::Name);
	gridLayout->addWidget(partnerBox, 8, 1, 1, 3);

	buyButton = new QPushButton(tr("Mark bought"));
	gridLayout->addWidget(buyButton, 10, 3, 1, 1);

	connect(buyButton, SIGNAL(clicked()), this, SLOT(buy()));
	
	connect(quantityEditor, SIGNAL(valueChanged(double)),
			this, SLOT(quantityValueChangedSlot(double)));
	connect(unitPriceEditor, SIGNAL(editingFinished()),
			this, SLOT(unitPriceEditingFinishedSlot()));
	connect(grossPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(grossPriceValueChangedSlot(double)));

	setLayout(gridLayout);

	loadState();
}

BuyItemView::~BuyItemView()
{
}

void BuyItemView::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
}

void BuyItemView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void BuyItemView::loadState()
{
	QSettings settings;
	QPoint pos = settings.value("buyitemview/position", QPoint()).toPoint();
	QSize size = settings.value("buyitemview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void BuyItemView::saveState()
{
	QSettings settings;
	settings.setValue("buyitemview/position", pos());
	settings.setValue("buyitemview/size", size());
}

void BuyItemView::setItem(unsigned itemRow, unsigned partnerRow)
{
	item = Item(model.item(itemRow));
	itemCursor = itemRow;
	partnerCursor = partnerRow;
}

void BuyItemView::mapToGui()
{
	nameLabel->setText(item.name + (item.type.size() ? (" (" + item.type) + ")" : ""));

	quantityEditor->blockSignals(true);
	quantityEditor->setValue(item.quantity);
	quantityEditor->blockSignals(false);

	unitPriceEditor->blockSignals(true);
	unitPriceEditor->setValue((DBL_EPSILON <= item.quantity) ?
			item.price / item.quantity : 0);
	unitPriceEditor->blockSignals(false);

	grossPriceEditor->blockSignals(true);
	grossPriceEditor->setValue(item.price);
	grossPriceEditor->blockSignals(false);

	partnerBox->setCurrentIndex(partnerCursor);
	invChangeDateTime->setDateTime(QDateTime::currentDateTime());
	
	WaresModel & wm = waresModel(dbname);
	int i = wm.index(item.name);
	if(i == -1) {
		quantityEditor->setSuffix("");
	} else {
		const Ware & w = wm.ware(i);
		quantityEditor->setSuffix(w.unit);
	}
}

void BuyItemView::mapFromGui()
{
	PartnersModel & pm = partnersModel(dbname);
	item.invChangeDate = invChangeDateTime->dateTime();
	item.quantity = quantityEditor->value();
	item.price = grossPriceEditor->value();
	item.partner = pm.partner(partnerBox->currentIndex()).name;
}

void BuyItemView::buy()
{
	mapFromGui();

	model.buy(itemCursor, item);
	accept();
}

void BuyItemView::quantityValueChangedSlot(double q)
{
	double u = unitPriceEditor->value();
	double g = grossPriceEditor->value();

	if(-DBL_EPSILON < g && g < DBL_EPSILON){
		grossPriceEditor->blockSignals(true);
		grossPriceEditor->setValue(u * q);
		grossPriceEditor->blockSignals(false);
		return;
	}

	if(-DBL_EPSILON < q && q < DBL_EPSILON){
		u = 0;
	} else {
		u = g / q;
	}

	unitPriceEditor->blockSignals(true);
	unitPriceEditor->setValue(u);
	unitPriceEditor->blockSignals(false);
}

void BuyItemView::unitPriceEditingFinishedSlot()
{
	double u = unitPriceEditor->value();
	double q = quantityEditor->value();
	double g = grossPriceEditor->value();

	if(-DBL_EPSILON < g && g < DBL_EPSILON){
		grossPriceEditor->blockSignals(true);
		grossPriceEditor->setValue(u * q);
		grossPriceEditor->blockSignals(false);
		return;
	}

	if(-DBL_EPSILON < u && u < DBL_EPSILON)
		q = 0;
	else
		q = g / u;

	quantityEditor->blockSignals(true);
	quantityEditor->setValue(q);
	quantityEditor->blockSignals(false);
}

void BuyItemView::grossPriceValueChangedSlot(double g)
{
	double u = 0;

	if(-DBL_EPSILON < g && g < DBL_EPSILON){
		quantityEditor->blockSignals(true);
		quantityEditor->setValue(0);
		quantityEditor->blockSignals(false);
	}

	double q = quantityEditor->value();
	if(-DBL_EPSILON < q && q < DBL_EPSILON){
		u = 0;
	} else {
		u = g / q;
	}

	unitPriceEditor->blockSignals(true);
	unitPriceEditor->setValue(u);
	unitPriceEditor->blockSignals(false);
}
