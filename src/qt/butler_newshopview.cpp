/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newshopview.h"
#include "butler_shopsmodel.h"

NewShopView::NewShopView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(shopsModel(dbname))
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Add new shop with details"));

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setColumnStretch(1, 1);

	QLabel *label = NULL;

	/* short name on the top */
	label = new QLabel(tr("Short name :"));
	gridLayout->addWidget(label, 0, 0, 1, 1);
	nameEditor = new QLineEdit;
	gridLayout->addWidget(nameEditor, 0, 1, 1, 3);

	/* store name */
	label = new QLabel(tr("Store name :"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	storeNameEditor = new QLineEdit;
	gridLayout->addWidget(storeNameEditor, 1, 1, 1, 3);

	/* city */
	label = new QLabel(tr("City :"));
	gridLayout->addWidget(label, 2, 0, 1, 1);
	cityEditor = new QLineEdit;
	gridLayout->addWidget(cityEditor, 2, 1, 1, 3);

	/* address */
	label = new QLabel(tr("Address :"));
	gridLayout->addWidget(label, 3, 0, 1, 1);
	addressEditor = new QLineEdit;
	gridLayout->addWidget(addressEditor, 3, 1, 1, 3);

	/* company */
	label = new QLabel(tr("Company :"));
	gridLayout->addWidget(label, 4, 0, 1, 1);
	companyEditor = new QLineEdit;
	gridLayout->addWidget(companyEditor, 4, 1, 1, 3);
	
	/* buttons: done */
	doneButton = new QPushButton;
	connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(doneClickedSlot(bool)));
	doneButton->setText(tr("Done"));
	gridLayout->addWidget(doneButton, 5, 3, 1, 1);

	setLayout(gridLayout);

	/* restore last state */
	loadState();
}

void NewShopView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);

	mapToGui();
}

void NewShopView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void NewShopView::loadState()
{
	QSettings settings;
	QPoint pos = settings.value("newshopview/position", QPoint()).toPoint();
	QSize size = settings.value("newshopview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void NewShopView::saveState()
{
	QSettings settings;
	settings.setValue("newshopview/position", pos());
	settings.setValue("newshopview/size", size());
}

void NewShopView::mapToGui()
{
	nameEditor->setText(shop.name);
	storeNameEditor->setText(shop.storeName);
	cityEditor->setText(shop.city);
	addressEditor->setText(shop.address);
	companyEditor->setText(shop.company);
}

void NewShopView::mapFromGui()
{
	shop.name = nameEditor->text();
	shop.storeName = storeNameEditor->text();
	shop.city = cityEditor->text();
	shop.address = addressEditor->text();
	shop.company = companyEditor->text();
}

void NewShopView::doneClickedSlot(bool toggled)
{
	Q_UNUSED(toggled);

	mapFromGui();
	model.addNew(shop);
	shop = Shop();
	mapToGui();
	accept();
}
