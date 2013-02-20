/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newitemview.h"
#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"


namespace Butler {

	NewItemView::NewItemView(QWidget *parent, ShoppingModel &m) :
		QDialog(parent),
		model(m)
	{
		setModal(true);
//		setWindowModality(Qt::ApplicationModal);
		setWindowTitle(tr("New item"));
		
		QLabel *label;

		QGridLayout *gridLayout = new QGridLayout();
		gridLayout->setColumnStretch(1, 1);
		setLayout(gridLayout);

		label = new QLabel(tr("Common name :"), this);
		gridLayout->addWidget(label, 0, 0, 1, 1);
		nameEditor = new QLineEdit;
		nameBox = new QComboBox;
		nameBox->setEditable(true);
		nameBox->setLineEdit(nameEditor);
		nameBox->setModel(&WaresModel::instance());
		nameBox->setModelColumn(WaresModel::Name);
		nameBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
		gridLayout->addWidget(nameBox, 0, 1, 1, 3);

		label = new QLabel(tr("Category name :"), this);
		gridLayout->addWidget(label, 1, 0, 1, 1);
		categoryEditor = new QLineEdit;
		categoryBox = new QComboBox;
		categoryBox->setEditable(true);
		categoryBox->setLineEdit(categoryEditor);
		categoryBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
		gridLayout->addWidget(categoryBox, 1, 1, 1, 3);

		label = new QLabel(tr("Quantity :"), this);
		gridLayout->addWidget(label, 3, 0, 1, 1);
		quantityEditor = new QDoubleSpinBox;
		quantityEditor->setRange(0, INT_MAX);
		quantityEditor->setDecimals(3);
		gridLayout->addWidget(quantityEditor, 3, 1, 1, 2);
		unitLabel = new QLabel(tr(""), this);
		gridLayout->addWidget(unitLabel, 3, 3, 1, 1);

		/* comments */
		label = new QLabel(tr("Comments:"));
		gridLayout->addWidget(label, 4, 0, 1, 4);

		commentEditor = new QTextEdit;
		gridLayout->addWidget(commentEditor, 5, 0, 1, 4);

		/* buttons: done */
		doneButton = new QPushButton;
		connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(doneClickedSlot(bool)));
		doneButton->setText(tr("Done"));
		gridLayout->addWidget(doneButton, 6, 3, 1, 1);

		connect(nameEditor, SIGNAL(editingFinished()),
				this, SLOT(nameEditFinishedSlot()));

		/* restore last state */
		loadState();
	}

	void NewItemView::showEvent(QShowEvent *event)
	{
		QDialog::showEvent(event);

		mapToGui();
	}

	void NewItemView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QDialog::closeEvent(event);
	}

	void NewItemView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("newitemview/position", QPoint()).toPoint();
		QSize size = settings.value("newitemview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void NewItemView::saveState()
	{
		QSettings settings(this);
		settings.setValue("newitemview/position", pos());
		settings.setValue("newitemview/size", size());
	}

	void NewItemView::mapToGui()
	{
		nameEditor->setText(item.name);
		nameEditFinishedSlot();
		categoryEditor->setText(item.category);
		quantityEditor->setValue(item.quantity);
		commentEditor->setText(item.comment);
	}

	void NewItemView::mapFromGui()
	{
		item.uploaded = QDateTime::currentDateTime();
		item.name = nameEditor->text();
		item.category = categoryEditor->text();
		item.quantity = quantityEditor->value();
		item.comment = commentEditor->toPlainText();
	}

	void NewItemView::doneClickedSlot(bool toggled)
	{
		Q_UNUSED(toggled);

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
					WARNING("Could not save new ware from edit item view.");
				}
			} else if(!wm.ware(i).categories.has(categoryEditor->text())) {
				Ware modified(wm.ware(i));
				modified.categories.add(new QString(categoryEditor->text()));
				if(!wm.update(i, modified)){
					WARNING("Could not save new ware category "
							"from edit item view.");
				}
			}
			item = Item();
			mapToGui();
			return accept();
		}

		QMessageBox(	QMessageBox::Warning,
				tr("Add new item failed"),
				model.error(),
				QMessageBox::Ok,
				0, Qt::Dialog).exec();
	}
	
	void NewItemView::nameEditFinishedSlot()
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
	
	void NewItemView::nameCurrentIndexChangedSlot(int index)
	{
		Q_UNUSED(index);

		nameEditFinishedSlot();
	}
}




