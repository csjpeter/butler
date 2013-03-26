/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newtagview.h"
#include "butler_tagsmodel.h"
#include "butler_tagsmodel.h"


NewTagView::NewTagView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(tagsModel(dbname))
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Add new tag"));
	
	QGridLayout *gridLayout = new QGridLayout();
	gridLayout->setColumnStretch(1, 1);

	QLabel *label = NULL;

	/* short name on the top */
	label = new QLabel(tr("Name:"));
	gridLayout->addWidget(label, 0, 0, 1, 1);
	nameEditor = new QLineEdit;
	gridLayout->addWidget(nameEditor, 0, 1, 1, 3);

	/* description of tag */
	label = new QLabel(tr("Description:"));
	gridLayout->addWidget(label, 1, 0, 1, 1);
	descEditor = new QLineEdit;
	gridLayout->addWidget(descEditor, 1, 1, 1, 3);
	
	/* buttons: done */
	doneButton = new QPushButton;
	connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(doneClickedSlot(bool)));
	doneButton->setText(tr("Done"));
	gridLayout->addWidget(doneButton, 3, 3, 1, 1);

	setLayout(gridLayout);

	/* restore last state */
	loadState();
}

void NewTagView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
}

void NewTagView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void NewTagView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("newtagview/position", QPoint()).toPoint();
	QSize size = settings.value("newtagview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void NewTagView::saveState()
{
	QSettings settings(this);
	settings.setValue("newtagview/position", pos());
	settings.setValue("newtagview/size", size());
}

void NewTagView::mapToGui()
{
	nameEditor->setText(tag.name);
	descEditor->setText(tag.description);
}

void NewTagView::mapFromGui()
{
	tag.name = nameEditor->text();
	tag.description = descEditor->text();
}

void NewTagView::doneClickedSlot(bool toggled)
{
	Q_UNUSED(toggled);

	mapFromGui();
	model.addNew(tag);
	tag = Tag();
	mapToGui();
	accept();
}
