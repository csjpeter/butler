/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include "butler_buyitemview.h"
#include "butler_itemsmodel.h"
#include "butler_waresmodel.h"
#include "butler_shopsmodel.h"

namespace Butler {

	BuyItemView::BuyItemView(QWidget *parent, ShoppingModel &m) :
		QDialog(parent),
		shoppingModel(m)
	{
		setModal(true);
//		setWindowModality(Qt::ApplicationModal);
		setWindowTitle(tr("Buy item"));
		
		QGridLayout *gridLayout = new QGridLayout();
		gridLayout->setColumnStretch(1, 1);
		setLayout(gridLayout);

		QLabel *label = NULL;

		label = new QLabel(tr("Name:"));
		gridLayout->addWidget(label, 1, 0, 1, 1);
		nameLabel = new QLabel();
		gridLayout->addWidget(nameLabel, 1, 1, 1, 3);

		label = new QLabel(tr("Quantity:"));
		gridLayout->addWidget(label, 3, 0, 1, 1);
		quantityEditor = new QDoubleSpinBox;
		quantityEditor->setRange(0, INT_MAX);
		quantityEditor->setDecimals(3);
		gridLayout->addWidget(quantityEditor, 3, 1, 1, 2);
		unitLabel = new QLabel(tr(""), this);
		gridLayout->addWidget(unitLabel, 3, 3, 1, 1);

		label = new QLabel(tr("Unit price:"));
		gridLayout->addWidget(label, 4, 0, 1, 1);
		unitPriceEditor = new QDoubleSpinBox;
		unitPriceEditor->setRange(0, INT_MAX);
		unitPriceEditor->setDecimals(2);
		gridLayout->addWidget(unitPriceEditor, 4, 1, 1, 3);

		label = new QLabel(tr("Gross price:"));
		gridLayout->addWidget(label, 5, 0, 1, 1);
		grossPriceEditor = new QDoubleSpinBox;
		grossPriceEditor->setRange(0, INT_MAX);
		grossPriceEditor->setDecimals(2);
		gridLayout->addWidget(grossPriceEditor, 5, 1, 1, 3);

		label = new QLabel(tr("Date of purchase:"));
		gridLayout->addWidget(label, 7, 0, 1, 1);
		purchaseDateTime = new QDateTimeEdit;
		purchaseDateTime->setCalendarPopup(true);
		purchaseDateTime->setDisplayFormat(Config::instance().dateTimeFormat());
		gridLayout->addWidget(purchaseDateTime, 7, 1, 1, 3);

		label = new QLabel(tr("Shop (place of buy):"));
		gridLayout->addWidget(label, 8, 0, 1, 1);
		shopBox = new QComboBox;
		shopBox->setModel(&ShopsModel::instance());
		shopBox->setModelColumn(Shop::Name);
		gridLayout->addWidget(shopBox, 8, 1, 1, 3);

		buyButton = new QPushButton(tr("Mark bought"), this);
		gridLayout->addWidget(buyButton, 10, 3, 1, 1);

		connect(buyButton, SIGNAL(clicked()), this, SLOT(buy()));
		
		connect(quantityEditor, SIGNAL(valueChanged(double)),
				this, SLOT(quantityValueChangedSlot(double)));
		connect(unitPriceEditor, SIGNAL(editingFinished()),
				this, SLOT(unitPriceEditingFinishedSlot()));
		connect(grossPriceEditor, SIGNAL(valueChanged(double)),
				this, SLOT(grossPriceValueChangedSlot(double)));

		loadState();
	}

	BuyItemView::~BuyItemView()
	{
	}

	void BuyItemView::showEvent(QShowEvent *event)
	{
		mapToGui();

		QDialog::showEvent(event);
	}

	void BuyItemView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QDialog::closeEvent(event);
	}

	void BuyItemView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("buyitemview/position", QPoint()).toPoint();
		QSize size = settings.value("buyitemview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void BuyItemView::saveState()
	{
		QSettings settings(this);
		settings.setValue("buyitemview/position", pos());
		settings.setValue("buyitemview/size", size());
	}
	
	void BuyItemView::setItem(unsigned itemRow, unsigned shopRow)
	{
		item = Item(shoppingModel.item(itemRow));
		itemCursor = itemRow;
		shopCursor = shopRow;
	}

	void BuyItemView::mapToGui()
	{
		nameLabel->setText(item.name + " (" + item.category + ")");

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

		shopBox->setCurrentIndex(shopCursor);
		purchaseDateTime->setDateTime(QDateTime::currentDateTime());
		
		WaresModel &wm = WaresModel::instance();
		int i = wm.index(item.name);
		if(i == -1) {
			unitLabel->setText("");
		} else {
			const Ware &w = wm.ware(i);
			unitLabel->setText(w.unit);
		}
	}

	void BuyItemView::mapFromGui()
	{
		item.purchased = purchaseDateTime->dateTime();
		item.quantity = quantityEditor->value();
		item.price = grossPriceEditor->value();
		item.shop = ShopsModel::instance().shop(shopBox->currentIndex()).name;
	}

	void BuyItemView::buy()
	{
		mapFromGui();

		if(shoppingModel.buy(itemCursor, item)){
			return accept();
		}

		QMessageBox(	QMessageBox::Warning,
				tr("Could not set item to bought."),
				shoppingModel.error(),
				QMessageBox::Ok,
				0, Qt::Dialog).exec();
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

}
