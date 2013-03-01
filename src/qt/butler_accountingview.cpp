/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

namespace Butler {

	AccountingView::AccountingView(QWidget *parent, ItemsModel &m) :
		QDialog(parent),
		model(m)
	{
		setModal(true);
//		setWindowModality(Qt::ApplicationModal);
		setWindowTitle(tr("Add already bought new item"));
		
		QLabel *label;

		QGridLayout *gridLayout = new QGridLayout();
		gridLayout->setColumnStretch(1, 1);
		setLayout(gridLayout);

		label = new QLabel(tr("Shop (place of buy) :"), this);
		gridLayout->addWidget(label, 0, 0, 1, 1);
		shopBox = new QComboBox;
		shopBox->setModel(&ShopsModel::instance());
		shopBox->setModelColumn(Shop::Name);
		gridLayout->addWidget(shopBox, 0, 1, 1, 2);

		label = new QLabel(tr("Purchase date :"), this);
		gridLayout->addWidget(label, 1, 0, 1, 1);
		purchaseDateTime = new QDateTimeEdit;
		purchaseDateTime->setCalendarPopup(true);
		purchaseDateTime->setDisplayFormat(Config::instance().dateTimeFormat());
		gridLayout->addWidget(purchaseDateTime, 1, 1, 1, 2);
		
		label = new QLabel("", this);
		gridLayout->addWidget(label, 2, 0, 1, 3);

		label = new QLabel(tr("Common name :"), this);
		gridLayout->addWidget(label, 3, 0, 1, 1);
		nameEditor = new QLineEdit;
		nameBox = new QComboBox;
		nameBox->setEditable(true);
		nameBox->setLineEdit(nameEditor);
		nameBox->setModel(&WaresModel::instance());
		nameBox->setModelColumn(WaresModel::Name);
		nameBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
		gridLayout->addWidget(nameBox, 3, 1, 1, 2);

		label = new QLabel(tr("Category name :"), this);
		gridLayout->addWidget(label, 4, 0, 1, 1);
		categoryEditor = new QLineEdit;
		categoryBox = new QComboBox;
		categoryBox->setEditable(true);
		categoryBox->setLineEdit(categoryEditor);
		categoryBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
		gridLayout->addWidget(categoryBox, 4, 1, 1, 2);

		label = new QLabel(tr("Quantity :"), this);
		gridLayout->addWidget(label, 5, 0, 1, 1);
		quantityEditor = new QDoubleSpinBox;
		quantityEditor->setRange(0, INT_MAX);
		quantityEditor->setDecimals(3);
		gridLayout->addWidget(quantityEditor, 5, 1, 1, 1);
		unitLabel = new QLabel(tr(""), this);
		gridLayout->addWidget(unitLabel, 5, 2, 1, 1);

		label = new QLabel(tr("Unit price :"), this);
		gridLayout->addWidget(label, 6, 0, 1, 1);
		unitPriceEditor = new QDoubleSpinBox;
		unitPriceEditor->setRange(0, INT_MAX);
		unitPriceEditor->setDecimals(2);
		gridLayout->addWidget(unitPriceEditor, 6, 1, 1, 2);

		label = new QLabel(tr("Gross price :"), this);
		gridLayout->addWidget(label, 7, 0, 1, 1);
		grossPriceEditor = new QDoubleSpinBox;
		grossPriceEditor->setRange(0, INT_MAX);
		grossPriceEditor->setDecimals(2);
		gridLayout->addWidget(grossPriceEditor, 7, 1, 1, 2);

		label = new QLabel(tr("On stock :"), this);
		gridLayout->addWidget(label, 8, 0, 1, 1);
		onStockCheck = new QCheckBox;
		gridLayout->addWidget(onStockCheck, 8, 1, 1, 2);
		
		label = new QLabel("", this);
		gridLayout->addWidget(label, 9, 0, 1, 3);

		label = new QLabel(tr("Upload date :"), this);
		gridLayout->addWidget(label, 10, 0, 1, 1);
		uploadDateTime = new QDateTimeEdit;
		uploadDateTime->setEnabled(false);
		uploadDateTime->setCalendarPopup(true);
		uploadDateTime->setDisplayFormat(Config::instance().dateTimeFormat());
		gridLayout->addWidget(uploadDateTime, 10, 1, 1, 2);

		label = new QLabel(tr("Bought :"), this);
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

		/* restore last state */
		loadState();
	}

	void AccountingView::showEvent(QShowEvent *event)
	{
		uploadDateTime->setDateTime(QDateTime::currentDateTime());

		QDialog::showEvent(event);

		mapToGui();
	}

	void AccountingView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QDialog::closeEvent(event);
	}

	void AccountingView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("accountingview/position", QPoint()).toPoint();
		QSize size = settings.value("accountingview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void AccountingView::saveState()
	{
		QSettings settings(this);
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
		ShopsModel &sm = ShopsModel::instance();
		if(0 <= i && i < sm.rowCount())
			item.shop = sm.shop(i).name;

		item.onStock = (onStockCheck->checkState() == Qt::Checked);
	}

	void AccountingView::saveSlot()
	{
		mapFromGui();

		if(model.addNew(item)){
			/* We want to save any new ware and category before closing dialog. */
			WaresModel &wm = WaresModel::instance();
			int i = wm.index(nameEditor->text());
			if(i == -1){
				Ware ware;
				ware.name = nameEditor->text();
				if(categoryEditor->text().size())
					ware.categories.add(new QString(categoryEditor->text()));
				if(!wm.addNew(ware)){
					QMessageBox(	QMessageBox::Warning,
							tr("Item added to db, "
							  "but adding new ware failed."),
							wm.error(),
							QMessageBox::Ok,
							0, Qt::Dialog).exec();
				}
			} else if(!wm.ware(i).categories.has(categoryEditor->text())) {
				Ware modified(wm.ware(i));
				modified.categories.add(new QString(categoryEditor->text()));
				if(!wm.update(i, modified)){
					QMessageBox(	QMessageBox::Warning,
							tr("Item added to db, "
							  "but adding new ware category failed."),
							wm.error(),
							QMessageBox::Ok,
							0, Qt::Dialog).exec();
				}
			}
			item = Item();
			item.uploaded = QDateTime::currentDateTime();
			mapToGui();
			nameEditor->setFocus(Qt::OtherFocusReason);
			return;
		} else {
			QMessageBox(	QMessageBox::Warning,
					tr("Could not add item to database."),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}
	}

	void AccountingView::nameEditFinishedSlot()
	{
		categoryBox->clear();

		WaresModel &wm = WaresModel::instance();
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
	
}



