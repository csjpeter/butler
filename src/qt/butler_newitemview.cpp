/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newitemview.h"
#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"


NewItemView::NewItemView(const QString & dbname, QWidget *parent) :
	PannView(parent),
	dbname(dbname),
	model(shoppingModel(dbname))
{
	setWindowModality(Qt::ApplicationModal);
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
	nameBox->setModel(&waresModel(dbname));
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
	PannView::showEvent(event);

	mapToGui();
}

void NewItemView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void NewItemView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("newitemview/position", QPoint()).toPoint();
	QSize size = settings.value("newitemview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
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
	model.addNew(item);
	/* We want to save any new ware and category before closing dialog. */
	WaresModel & wm = waresModel(dbname);
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
	mapToGui();
	hide();
}

void NewItemView::nameEditFinishedSlot()
{
	categoryBox->clear();

	WaresModel & wm = waresModel(dbname);
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
