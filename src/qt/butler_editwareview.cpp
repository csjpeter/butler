/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editwareview.h"
#include "butler_waresmodel.h"
#include "butler_tagsmodel.h"
#include "butler_tagwidget.h"

EditWareView::EditWareView(const QString & dbname, QWidget *parent) :
	QDialog(parent),
	dbname(dbname),
	model(waresModel(dbname))
{
	setModal(true);
//	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Edit ware details"));

	QLabel *label = NULL;
	QHBoxLayout *hbox;

	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);
	
	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->setColumnStretch(1, 1);
	layout->addLayout(gridLayout);

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
	tagsSelector = new TagWidget(dbname, this);
	gridLayout->addWidget(tagsSelector, 4, 0, 1, 4);

	/* buttons: prev, save, next, close */
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
	
	closeButton = new QPushButton;
	closeButton->setText(tr("&Close"));
	closeButton->setAutoDefault(true);
	closeButton->setDefault(true);
	layout->addWidget(closeButton);

	/* restore last state */
	loadState();

	connect(prevButton, SIGNAL(clicked()),
			this, SLOT(prevClickedSlot()));
	connect(nextButton, SIGNAL(clicked()),
			this, SLOT(nextClickedSlot()));
	connect(saveButton, SIGNAL(clicked()),
			this, SLOT(saveSlot()));
	connect(closeButton, SIGNAL(clicked()),
			this, SLOT(closeSlot()));
}

void EditWareView::showEvent(QShowEvent *event)
{
	tagsModel(dbname).query();
	
	mapToGui();
	
	QDialog::showEvent(event);
}

void EditWareView::closeEvent(QCloseEvent *event)
{
	saveState();

	QDialog::closeEvent(event);
}

void EditWareView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("editwareview/position", QPoint()).toPoint();
	QSize size = settings.value("editwareview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	move(pos);
}

void EditWareView::saveState()
{
	QSettings settings(this);
	settings.setValue("editwareview/position", pos());
	settings.setValue("editwareview/size", size());
}

void EditWareView::mapToGui()
{
	ware = Ware(model.ware(cursor.row()));

	nameEditor->setText(ware.name);
	unitEditor->setText(ware.unit);
	categoriesEditor->setText(WaresModel::categoriesToString(ware.categories));
	tagsSelector->setTags(ware.tags);
}

void EditWareView::mapFromGui()
{
	ware.name = nameEditor->text();
	ware.unit = unitEditor->text();
	WaresModel::stringToCategories(categoriesEditor->text(), ware.categories);
	tagsSelector->getTags(ware.tags);
}

void EditWareView::setCursor(const QModelIndex& index)
{
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	mapToGui();
	prevButton->setEnabled(cursor.row() > 0);
	nextButton->setEnabled(cursor.row() < model.rowCount() - 1);
}

void EditWareView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditWareView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditWareView::saveSlot()
{
	mapFromGui();
	model.update(cursor.row(), ware);
/*
	QMessageBox(	QMessageBox::Warning,
			tr("Update ware failed"),
			model.error(),
			QMessageBox::Ok,
			0, Qt::Dialog).exec();
*/
}

void EditWareView::closeSlot()
{
	accept();
}
