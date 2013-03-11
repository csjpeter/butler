/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editshopview.h"
#include "butler_shopsmodel.h"

EditShopView::EditShopView(const QString & dbname, QWidget *parent) :
	QDialog(parent),
	dbname(dbname),
	model(shopsModel(dbname))
{
	setModal(true);
//	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Edit shop details"));

	QLabel *label = NULL;
	QHBoxLayout *hbox;

	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);
	
	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->setColumnStretch(1, 1);
	layout->addLayout(gridLayout);

	/* short name on the top */
	label = new QLabel(tr("Short name :"));
	gridLayout->addWidget(label, 0, 0, 1, 1);
	nameEditor = new QLineEdit;
	gridLayout->addWidget(nameEditor, 0, 1, 1, 2);

	/* store name */
	label = new QLabel(tr("Store name :"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	storeNameEditor = new QLineEdit;
	gridLayout->addWidget(storeNameEditor, 1, 1, 1, 2);

	/* city */
	label = new QLabel(tr("City :"));
	gridLayout->addWidget(label, 2, 0, 1, 1);
	cityEditor = new QLineEdit;
	gridLayout->addWidget(cityEditor, 2, 1, 1, 2);

	/* address */
	label = new QLabel(tr("Address :"));
	gridLayout->addWidget(label, 3, 0, 1, 1);
	addressEditor = new QLineEdit;
	gridLayout->addWidget(addressEditor, 3, 1, 1, 2);

	/* company */
	label = new QLabel(tr("Company :"));
	gridLayout->addWidget(label, 4, 0, 1, 1);
	companyEditor = new QLineEdit;
	gridLayout->addWidget(companyEditor, 4, 1, 1, 2);

	/* buttons: prev, save, next */
	hbox = new QHBoxLayout();
	layout->addLayout(hbox);

	prevButton = new QPushButton;
	prevButton->setText(tr("Prev"));
	prevButton->setAutoDefault(false);
	hbox->addWidget(prevButton);

	saveButton = new QPushButton;
	saveButton->setText(tr("Save"));
	saveButton->setAutoDefault(false);
	saveButton->setDefault(true);
	hbox->addWidget(saveButton);

	nextButton = new QPushButton;
	nextButton->setText(tr("Next"));
	nextButton->setAutoDefault(false);
	hbox->addWidget(nextButton);

	/* restore last state */
	loadState();

	connect(prevButton, SIGNAL(clicked()),
			this, SLOT(prevClickedSlot()));
	connect(nextButton, SIGNAL(clicked()),
			this, SLOT(nextClickedSlot()));
	connect(saveButton, SIGNAL(clicked()),
			this, SLOT(saveSlot()));
}

void EditShopView::showEvent(QShowEvent *event)
{
	QDialog::showEvent(event);
}

void EditShopView::closeEvent(QCloseEvent *event)
{
	saveState();

	QDialog::closeEvent(event);
}

void EditShopView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("editshopview/position", QPoint()).toPoint();
	QSize size = settings.value("editshopview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void EditShopView::saveState()
{
	QSettings settings(this);
	settings.setValue("editshopview/position", pos());
	settings.setValue("editshopview/size", size());
}

void EditShopView::mapToGui()
{
	shop = Shop(model.shop(cursor.row()));

	nameEditor->setText(shop.name);
	storeNameEditor->setText(shop.storeName);
	cityEditor->setText(shop.city);
	addressEditor->setText(shop.address);
	companyEditor->setText(shop.company);
}

void EditShopView::mapFromGui()
{
	shop.name = nameEditor->text();
	shop.storeName = storeNameEditor->text();
	shop.city = cityEditor->text();
	shop.address = addressEditor->text();
	shop.company = companyEditor->text();
}

void EditShopView::setCursor(const QModelIndex& index)
{
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	mapToGui();
	prevButton->setEnabled(cursor.row() > 0);
	nextButton->setEnabled(cursor.row() < model.rowCount() - 1);
}

void EditShopView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditShopView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditShopView::saveSlot()
{
	mapFromGui();
	model.update(cursor.row(), shop);
	accept();
}
