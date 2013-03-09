/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newwareview.h"
#include "butler_waresmodel.h"
#include "butler_tagwidget.h"

NewWareView::NewWareView(QWidget *parent, WaresModel &m) :
	QDialog(parent),
	model(m)
{
	setModal(true);
//	setWindowModality(Qt::WindowModal);
	setWindowTitle(tr("Add new ware with details"));
	
	QLabel *label = NULL;

	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->setColumnStretch(1, 1);
	setLayout(gridLayout);

	/* ware name */
	label = new QLabel(tr("Name :"));
	gridLayout->addWidget(label, 0, 0, 1, 1);
	nameEditor = new QLineEdit;
	gridLayout->addWidget(nameEditor, 0, 1, 1, 3);

	/* unit */
	label = new QLabel(tr("Unit :"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	unitEditor = new QLineEdit;
	gridLayout->addWidget(unitEditor, 1, 1, 1, 3);

	/* categories */
	label = new QLabel(tr("Categories :"));
	gridLayout->addWidget(label, 2, 0, 1, 1);
	categoriesEditor = new QLineEdit;
	gridLayout->addWidget(categoriesEditor, 2, 1, 1, 3);

	/* tags */
	label = new QLabel(tr("Tags :"));
	gridLayout->addWidget(label, 3, 0, 1, 4);
	tagsSelector = new TagWidget(this, tagsModel(model.dbname).tagSet());
	gridLayout->addWidget(tagsSelector, 4, 0, 1, 4);
	
	/* buttons: done */
	doneButton = new QPushButton;
	connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(doneClickedSlot(bool)));
	doneButton->setText(tr("Done"));
	gridLayout->addWidget(doneButton, 5, 3, 1, 1);

	/* restore last state */
	loadState();
}

void NewWareView::showEvent(QShowEvent *event)
{
	tagsModel(model.dbname).query();
	
	QDialog::showEvent(event);

	mapToGui();
}

void NewWareView::closeEvent(QCloseEvent *event)
{
	saveState();

	QDialog::closeEvent(event);
}

void NewWareView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("newwareview/position", QPoint()).toPoint();
	QSize size = settings.value("newwareview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	move(pos);
}

void NewWareView::saveState()
{
	QSettings settings(this);
	settings.setValue("newwareview/position", pos());
	settings.setValue("newwareview/size", size());
}

void NewWareView::mapToGui()
{
	nameEditor->setText(ware.name);
	unitEditor->setText(ware.unit);
	categoriesEditor->setText(WaresModel::categoriesToString(ware.categories));
	tagsSelector->setTags(ware.tags);
}

void NewWareView::mapFromGui()
{
	ware.name = nameEditor->text();
	ware.unit = unitEditor->text();
	WaresModel::stringToCategories(categoriesEditor->text(), ware.categories);
	tagsSelector->getTags(ware.tags);
}

void NewWareView::doneClickedSlot(bool toggled)
{
	Q_UNUSED(toggled);

	mapFromGui();
	model.addNew(ware);
	ware = Ware();
	mapToGui();
	accept();
/*
	QMessageBox(	QMessageBox::Warning,
			tr("Add new ware failed"),
			model.error(),
			QMessageBox::Ok,
			0, Qt::Dialog).exec();*/
}
