/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_edititemview.h"
#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"
#include "butler_shopsmodel.h"

EditItemView::EditItemView(const QString & dbname, ItemsModel & model, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(model)
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Edit item details"));
	
	QHBoxLayout *hbox;
	QLabel *label;

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->setColumnStretch(1, 1);

	label = new QLabel(tr("Common name :"));
	gridLayout->addWidget(label, 0, 0, 1, 1);
	nameEditor = new QLineEdit;
	nameBox = new QComboBox;
	nameBox->setEditable(true);
	nameBox->setLineEdit(nameEditor);
	nameBox->setModel(&waresModel(dbname));
	nameBox->setModelColumn(WaresModel::Name);
	nameBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
	gridLayout->addWidget(nameBox, 0, 1, 1, 2);

	label = new QLabel(tr("Category name :"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	categoryEditor = new QLineEdit;
	categoryBox = new QComboBox;
	categoryBox->setEditable(true);
	categoryBox->setLineEdit(categoryEditor);
	categoryBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
	gridLayout->addWidget(categoryBox, 1, 1, 1, 2);

	label = new QLabel(tr("Upload date :"));
	gridLayout->addWidget(label, 2, 0, 1, 1);
	uploadDateTime = new QDateTimeEdit;
	uploadDateTime->setEnabled(false);
	uploadDateTime->setCalendarPopup(true);
	uploadDateTime->setDisplayFormat(Config::dateTimeFormat());
	gridLayout->addWidget(uploadDateTime, 2, 1, 1, 2);

	label = new QLabel(tr("Quantity :"));
	gridLayout->addWidget(label, 3, 0, 1, 1);
	quantityEditor = new QDoubleSpinBox;
	quantityEditor->setRange(0, INT_MAX);
	quantityEditor->setDecimals(3);
	gridLayout->addWidget(quantityEditor, 3, 1, 1, 1);
	unitLabel = new QLabel;
	gridLayout->addWidget(unitLabel, 3, 2, 1, 1);

	label = new QLabel(tr("Bought :"));
	gridLayout->addWidget(label, 4, 0, 1, 1);
	boughtCheck = new QCheckBox;
	gridLayout->addWidget(boughtCheck, 4, 1, 1, 2);

	label = new QLabel(tr("Unit price :"));
	gridLayout->addWidget(label, 5, 0, 1, 1);
	unitPriceEditor = new QDoubleSpinBox;
	unitPriceEditor->setRange(0, INT_MAX);
	unitPriceEditor->setDecimals(2);
	gridLayout->addWidget(unitPriceEditor, 5, 1, 1, 2);

	label = new QLabel(tr("Gross price :"));
	gridLayout->addWidget(label, 6, 0, 1, 1);
	grossPriceEditor = new QDoubleSpinBox;
	grossPriceEditor->setRange(0, INT_MAX);
	grossPriceEditor->setDecimals(2);
	gridLayout->addWidget(grossPriceEditor, 6, 1, 1, 2);

	label = new QLabel(tr("Purchase date :"));
	gridLayout->addWidget(label, 7, 0, 1, 1);
	purchaseDateTime = new QDateTimeEdit;
	purchaseDateTime->setCalendarPopup(true);
	purchaseDateTime->setDisplayFormat(Config::dateTimeFormat());
	gridLayout->addWidget(purchaseDateTime, 7, 1, 1, 2);

	label = new QLabel(tr("Shop (place of buy) :"));
	gridLayout->addWidget(label, 8, 0, 1, 1);
	shopBox = new QComboBox;
	shopBox->setModel(&shopsModel(dbname));
	shopBox->setModelColumn(Shop::Name);
	gridLayout->addWidget(shopBox, 8, 1, 1, 2);

	label = new QLabel(tr("On stock :"));
	gridLayout->addWidget(label, 9, 0, 1, 1);
	onStockCheck = new QCheckBox;
	gridLayout->addWidget(onStockCheck, 9, 1, 1, 2);

	/* buttons: prev, save, next */
	hbox = new QHBoxLayout;
	gridLayout->addLayout(hbox, 10, 0, 1, 3);

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

	/* comment editor */
	label = new QLabel(tr("Comments:"));
	gridLayout->addWidget(label, 11, 0, 1, 3);

	commentEditor = new QTextEdit;
	gridLayout->addWidget(commentEditor, 12, 0, 1, 3);

	connect(prevButton, SIGNAL(clicked()),
			this, SLOT(prevClickedSlot()));
	connect(nextButton, SIGNAL(clicked()),
			this, SLOT(nextClickedSlot()));
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

void EditItemView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);

	mapToGui();
}

void EditItemView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void EditItemView::loadState()
{
	QSettings settings;
	QPoint pos = settings.value("edititemview/position", QPoint()).toPoint();
	QSize size = settings.value("edititemview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void EditItemView::saveState()
{
	QSettings settings;
	settings.setValue("edititemview/position", pos());
	settings.setValue("edititemview/size", size());
}

void EditItemView::mapToGui()
{
	updatedItem = Item(model.item(cursor.row()));

	uploadDateTime->setDateTime(updatedItem.uploaded);

	nameEditor->setText(updatedItem.name);
	nameEditFinishedSlot();
	categoryEditor->setText(updatedItem.category);

	quantityEditor->blockSignals(true);
	quantityEditor->setValue(updatedItem.quantity);
	quantityEditor->blockSignals(false);

	commentEditor->setText(updatedItem.comment);

	boughtCheck->setCheckState(updatedItem.bought ? Qt::Checked : Qt::Unchecked);

	unitPriceEditor->blockSignals(true);
	unitPriceEditor->setValue((DBL_EPSILON <= updatedItem.quantity) ?
			updatedItem.price / updatedItem.quantity : 0);
	unitPriceEditor->blockSignals(false);

	grossPriceEditor->blockSignals(true);
	grossPriceEditor->setValue(updatedItem.price);
	grossPriceEditor->blockSignals(false);

	purchaseDateTime->setDateTime(updatedItem.purchased);
	shopBox->setCurrentIndex(shopsModel(dbname).index(updatedItem.shop));

	onStockCheck->setCheckState(updatedItem.onStock ? Qt::Checked : Qt::Unchecked);
}

void EditItemView::mapFromGui()
{
	updatedItem.uploaded = uploadDateTime->dateTime();

	updatedItem.name = nameEditor->text();
	updatedItem.category = categoryEditor->text();
	updatedItem.quantity = quantityEditor->value();
	updatedItem.comment = commentEditor->toPlainText();

	updatedItem.bought = (boughtCheck->checkState() == Qt::Checked);
	updatedItem.price = grossPriceEditor->value();
	updatedItem.purchased = purchaseDateTime->dateTime();

	int i = shopBox->currentIndex();
	ShopsModel &sm = shopsModel(dbname);
	if(0 <= i && i < sm.rowCount())
		updatedItem.shop = sm.shop(i).name;

	updatedItem.onStock = (onStockCheck->checkState() == Qt::Checked);
}

void EditItemView::setCursor(const QModelIndex& index)
{
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	mapToGui();
	prevButton->setEnabled(cursor.row() > 0);
	nextButton->setEnabled(cursor.row() < model.rowCount() - 1);
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
	
	model.update(cursor.row(), updatedItem);
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
	/* May be it is better to jump to next record than as to close the view. */
	nextClickedSlot();
	accept();
}

void EditItemView::nameEditFinishedSlot()
{
	categoryBox->clear();

	WaresModel &wm = waresModel(dbname);
	int i = wm.index(nameEditor->text());
	if(i == -1){
		unitLabel->setText("");
		return;
	}

	const Ware &w = wm.ware(i);

	QString cats = WaresModel::categoriesToString(w.categories);
	categoryBox->addItems(cats.split(", ", QString::SkipEmptyParts));
	
	unitLabel->setText(w.unit);
}

void EditItemView::quantityValueChangedSlot(double q)
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

void EditItemView::unitPriceEditingFinishedSlot()
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

void EditItemView::grossPriceValueChangedSlot(double g)
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
