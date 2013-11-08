/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newitemview.h"
#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"


NewItemView::NewItemView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(shoppingModel(dbname))
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("New item"));
	
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
	nameBox->setModelColumn(Ware::Name);
	nameBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
	gridLayout->addWidget(nameBox, 0, 1, 1, 3);

	label = new QLabel(tr("Type name :"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	typeEditor = new QLineEdit;
	typeBox = new QComboBox;
	typeBox->setEditable(true);
	typeBox->setLineEdit(typeEditor);
	typeBox->completer()->setCompletionMode(QCompleter::PopupCompletion);
	gridLayout->addWidget(typeBox, 1, 1, 1, 3);

	label = new QLabel(tr("Quantity :"));
	gridLayout->addWidget(label, 3, 0, 1, 1);
	quantityEditor = new QDoubleSpinBox;
	quantityEditor->setRange(0, INT_MAX);
	quantityEditor->setDecimals(3);
	quantityEditor->setButtonSymbols(QAbstractSpinBox::NoButtons);
	gridLayout->addWidget(quantityEditor, 3, 1, 1, 3);

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

	setLayout(gridLayout);

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
	QSettings settings;
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
	QSettings settings;
	settings.setValue("newitemview/position", pos());
	settings.setValue("newitemview/size", size());
}

void NewItemView::mapToGui()
{
	nameEditor->setText(item.name);
	nameEditFinishedSlot();
	typeEditor->setText(item.type);
	quantityEditor->setValue(item.quantity);
	commentEditor->setText(item.comment);
}

void NewItemView::mapFromGui()
{
	item.uploaded = QDateTime::currentDateTime();
	item.name = nameEditor->text();
	item.type = typeEditor->text();
	item.quantity = quantityEditor->value();
	item.comment = commentEditor->toPlainText();
}

void NewItemView::doneClickedSlot(bool toggled)
{
	Q_UNUSED(toggled);

	mapFromGui();
	model.addNew(item);
	/* We want to save any new ware and type before closing dialog. */
	WaresModel & wm = waresModel(dbname);
	int i = wm.index(nameEditor->text());
	if(i == -1){
		Ware ware;
		ware.name <<= nameEditor->text();
		if(typeEditor->text().size())
			ware.types.add(new Text(typeEditor->text()));
		wm.addNew(ware);
	} else if(!wm.ware(i).types.has(typeEditor->text())) {
		Ware modified(wm.ware(i));
		modified.types.add(new Text(typeEditor->text()));
		wm.update(i, modified);
	}
	item = Item();
	mapToGui();
	hide();
}

void NewItemView::nameEditFinishedSlot()
{
	typeBox->clear();

	WaresModel & wm = waresModel(dbname);
	int i = wm.index(nameEditor->text());
	if(i == -1){
		quantityEditor->setSuffix("");
		return;
	}

	const Ware & w = wm.ware(i);

	QString cats = WaresModel::typesToString(w.types);
	typeBox->addItems(cats.split(", ", QString::SkipEmptyParts));
	
	quantityEditor->setSuffix(w.unit);
}

void NewItemView::nameCurrentIndexChangedSlot(int index)
{
	Q_UNUSED(index);

	nameEditFinishedSlot();
}
