/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_tagfilterview.h"
#include "butler_tagwidget.h"

TagFilterView::TagFilterView(const QString & dbname, TagNameSet &tags, QWidget *parent) :
	QDialog(parent),
	dbname(dbname),
	tags(tags)
{
	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);

	QLabel *label = NULL;

	label = new QLabel(tr("Tags :"));
	gridLayout->addWidget(label, 0, 0, 1, 4);
	tagsSelector = new TagWidget(dbname, this);
	gridLayout->addWidget(tagsSelector, 1, 0, 1, 4);

	selectAllButton = new QPushButton;
	connect(selectAllButton, SIGNAL(clicked(bool)),
			this, SLOT(selectAllClickedSlot(bool)));
	selectAllButton->setText(tr("Select all tags"));
	gridLayout->addWidget(selectAllButton, 2, 1, 1, 1);

	selectNoneButton = new QPushButton;
	connect(selectNoneButton, SIGNAL(clicked(bool)),
			this, SLOT(selectNoneClickedSlot(bool)));
	selectNoneButton->setText(tr("Deselect all tags"));
	gridLayout->addWidget(selectNoneButton, 2, 2, 1, 1);

	okButton = new QPushButton;
	connect(okButton, SIGNAL(clicked(bool)), this, SLOT(okClickedSlot(bool)));
	okButton->setText(tr("Ok"));
	gridLayout->addWidget(okButton, 3, 3, 1, 1);

	/* restore last state */
	loadState();
}

TagFilterView::~TagFilterView()
{
}

void TagFilterView::showEvent(QShowEvent *event)
{
	tagsModel(dbname).query();
	
	mapToGui();

	QWidget::showEvent(event);
}

void TagFilterView::closeEvent(QCloseEvent *event)
{
	saveState();

	QWidget::closeEvent(event);
}

void TagFilterView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("tagsfilterview/position", QPoint()).toPoint();
	QSize size = settings.value("tagsfilterview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	move(pos);
}

void TagFilterView::saveState()
{
	QSettings settings(this);
	settings.setValue("tagsfilterview/position", pos());
	settings.setValue("tagsfilterview/size", size());
}

void TagFilterView::mapToGui()
{
	tagsSelector->setTags(tags);
}

void TagFilterView::mapFromGui()
{
	tagsSelector->getTags(tags);
}

void TagFilterView::okClickedSlot(bool)
{
	mapFromGui();

	accept();
}

void TagFilterView::selectAllClickedSlot(bool)
{
	tagsSelector->selectAll();
}

void TagFilterView::selectNoneClickedSlot(bool)
{
	tagsSelector->deselectAll();
}
