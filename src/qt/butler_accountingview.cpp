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
	model(model)
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Add already bought new item"));
	
	QLabel *label;

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setColumnStretch(1, 1);

	label = new QLabel(tr("Shop (place of buy) :"));
	gridLayout->addWidget(label, 0, 0, 1, 1);
	shopBox = new QComboBox;
	shopBox->setModel(&shopsModel(dbname));
	shopBox->setModelColumn(Shop::Name);
	gridLayout->addWidget(shopBox, 0, 1, 1, 2);

	label = new QLabel(tr("Purchase date :"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	purchaseDateTime = new MyDateTimeEdit;
	purchaseDateTime->setCalendarPopup(true);
	purchaseDateTime->setDisplayFormat(Config::dateTimeFormat());
	gridLayout->addWidget(purchaseDateTime, 1, 1, 1, 2);
	
	label = new QLabel;
	gridLayout->addWidget(label, 2, 0, 1, 3);

	label = new QLabel(tr("Common name :"));
	gridLayout->addWidget(label, 3, 0, 1, 1);
	nameEditor = new QLineEdit;
	nameBox = new QComboBox;
	nameBox->setEditable(true);
	nameBox->setLineEdit(nameEditor);
	nameBox->setModel(&waresModel(dbname));
	nameBox->setModelColumn(WaresModel::Name);
	nameBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
	gridLayout->addWidget(nameBox, 3, 1, 1, 2);

	label = new QLabel(tr("Category name :"));
	gridLayout->addWidget(label, 4, 0, 1, 1);
	categoryEditor = new QLineEdit;
	categoryBox = new QComboBox;
	categoryBox->setEditable(true);
	categoryBox->setLineEdit(categoryEditor);
	categoryBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
	gridLayout->addWidget(categoryBox, 4, 1, 1, 2);

	label = new QLabel(tr("Quantity :"));
	gridLayout->addWidget(label, 5, 0, 1, 1);
	quantityEditor = new QDoubleSpinBox;
	quantityEditor->setRange(0, INT_MAX);
	quantityEditor->setDecimals(3);
	quantityEditor->setFrame(false);
	quantityEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(quantityEditor, 5, 1, 1, 2);

	label = new QLabel(tr("Unit price :"));
	gridLayout->addWidget(label, 6, 0, 1, 1);
	unitPriceEditor = new QDoubleSpinBox;
	unitPriceEditor->setRange(0, INT_MAX);
	unitPriceEditor->setDecimals(2);
	unitPriceEditor->setFrame(false);
	unitPriceEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(unitPriceEditor, 6, 1, 1, 2);

	label = new QLabel(tr("Gross price :"));
	gridLayout->addWidget(label, 7, 0, 1, 1);
	grossPriceEditor = new MySpinBox;
	grossPriceEditor->setRange(0, INT_MAX);
	grossPriceEditor->setDecimals(2);
	grossPriceEditor->setFrame(false);
	grossPriceEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(grossPriceEditor, 7, 1, 1, 2);

	label = new QLabel(tr("On stock :"));
	gridLayout->addWidget(label, 8, 0, 1, 1);
	onStockCheck = new QCheckBox;
	gridLayout->addWidget(onStockCheck, 8, 1, 1, 2);
	
	label = new QLabel;
	gridLayout->addWidget(label, 9, 0, 1, 3);

	label = new QLabel(tr("Upload date :"));
	gridLayout->addWidget(label, 10, 0, 1, 1);
	uploadDateTime = new MyDateTimeEdit;
	uploadDateTime->setEnabled(false);
	uploadDateTime->setCalendarPopup(true);
	uploadDateTime->setDisplayFormat(Config::dateTimeFormat());
	gridLayout->addWidget(uploadDateTime, 10, 1, 1, 2);

	label = new QLabel(tr("Bought :"));
	gridLayout->addWidget(label, 11, 0, 1, 1);
	boughtCheck = new QCheckBox;
	boughtCheck->setCheckState(Qt::Checked);
	gridLayout->addWidget(boughtCheck, 11, 1, 1, 2);


	/* save button */
	saveButton = new QPushButton;
	saveButton->setText(tr("Save"));
	gridLayout->addWidget(saveButton, 12, 1, 1, 1);

	/* comment editor */
	label = new QLabel(tr("Comments:"));
	gridLayout->addWidget(label, 13, 0, 1, 3);

	commentEditor = new QTextEdit;
	gridLayout->addWidget(commentEditor, 14, 0, 1, 3);

	connect(saveButton, SIGNAL(clicked()),
			this, SLOT(saveSlot()));
	
	connect(nameEditor, SIGNAL(editingFinished()),
			this, SLOT(nameEditFinishedSlot()));
	connect(quantityEditor, SIGNAL(valueChanged(double)),
			this, SLOT(quantityValueChangedSlot(double)));
	connect(unitPriceEditor, SIGNAL(editingFinished()),
			this, SLOT(unitPriceEditingFinishedSlot()));
	connect(grossPriceEditor, SIGNAL(valueChanged(double)),
			this, SLOT(grossPriceValueChangedSlot(double)));

	setLayout(gridLayout);

	/* restore last state */
	loadState();
}

void AccountingView::showEvent(QShowEvent *event)
{
	uploadDateTime->setDateTime(QDateTime::currentDateTime());

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
	uploadDateTime->setDateTime(item.uploaded);

	nameEditor->setText(item.name);
	nameEditFinishedSlot();
	categoryEditor->setText(item.category);

	quantityEditor->blockSignals(true);
	quantityEditor->setValue(item.quantity);
	quantityEditor->blockSignals(false);

	commentEditor->setText(item.comment);

	unitPriceEditor->blockSignals(true);
	unitPriceEditor->setValue((DBL_EPSILON <= item.quantity) ?
			item.price / item.quantity : 0);
	unitPriceEditor->blockSignals(false);

	grossPriceEditor->blockSignals(true);
	grossPriceEditor->setValue(item.price);
	grossPriceEditor->blockSignals(false);

	onStockCheck->setCheckState(item.onStock ? Qt::Checked : Qt::Unchecked);
}

void AccountingView::mapFromGui()
{
	item.uploaded = uploadDateTime->dateTime();

	item.name = nameEditor->text();
	item.category = categoryEditor->text();
	item.quantity = quantityEditor->value();
	item.comment = commentEditor->toPlainText();

	item.bought = (boughtCheck->checkState() == Qt::Checked);
	item.price = grossPriceEditor->value();
	item.purchased = purchaseDateTime->dateTime();

	int i = shopBox->currentIndex();
	ShopsModel &sm = shopsModel(dbname);
	if(0 <= i && i < sm.rowCount())
		item.shop = sm.shop(i).name;

	item.onStock = (onStockCheck->checkState() == Qt::Checked);
}

void AccountingView::saveSlot()
{
	mapFromGui();

	model.addNew(item);

	/* We want to save any new ware and category before closing dialog. */
	WaresModel &wm = waresModel(dbname);
	int i = wm.index(nameEditor->text());
	if(i == -1){
		Ware ware;
		ware.name = nameEditor->text();
		if(categoryEditor->text().size())
			ware.categories.add(new QString(categoryEditor->text()));
		wm.addNew(ware);
	} else if(!wm.ware(i).categories.has(categoryEditor->text())) {
		Ware modified(wm.ware(i));
		modified.categories.add(new QString(categoryEditor->text()));
		wm.update(i, modified);
	}

	item = Item();
	item.uploaded = QDateTime::currentDateTime();
	mapToGui();
	nameEditor->setFocus(Qt::OtherFocusReason);
}

void AccountingView::nameEditFinishedSlot()
{
	categoryBox->clear();

	WaresModel &wm = waresModel(dbname);
	int i = wm.index(nameEditor->text());
	if(i == -1){
		quantityEditor->setSuffix("");
		return;
	}

	const Ware &w = wm.ware(i);

	QString cats = WaresModel::categoriesToString(w.categories);
	categoryBox->addItems(cats.split(", ", QString::SkipEmptyParts));

	quantityEditor->setSuffix(w.unit);
}

void AccountingView::quantityValueChangedSlot(double q)
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

void AccountingView::unitPriceEditingFinishedSlot()
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

void AccountingView::grossPriceValueChangedSlot(double g)
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
