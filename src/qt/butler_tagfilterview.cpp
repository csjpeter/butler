/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_tagfilterview.h"
#include "butler_tagwidget.h"

TagFilterView::TagFilterView(const QString & dbname, TagNameSet &tags, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	tags(tags)
{
	setWindowModality(Qt::ApplicationModal);
	setWindowTitle(tr("Tag filter view"));

	QGridLayout *gridLayout = new QGridLayout;

	QLabel *label = NULL;

	label = new QLabel(tr("Tags :"));
	gridLayout->addWidget(label, 0, 0, 1, 4);
	tagsSelector = new TagWidget(dbname);
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
	
	setLayout(gridLayout);
	loadState();
}

TagFilterView::~TagFilterView()
{
}

void TagFilterView::showEvent(QShowEvent *event)
{
	mapToGui();
	PannView::showEvent(event);
}

void TagFilterView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void TagFilterView::resizeEvent(QResizeEvent *event)
{
	PannView::resizeEvent(event);
	QTimer::singleShot(0, tagsSelector, SLOT(prepareContent()));
}

void TagFilterView::loadState()
{
	QString prefix("TagFilterView");
	PannView::loadState(prefix);
}

void TagFilterView::saveState()
{
	QString prefix("TagFilterView");
	PannView::saveState(prefix);
}

void TagFilterView::mapToGui()
{
	tagsSelector->setTags(tags);
}

void TagFilterView::mapFromGui()
{
	tags = tagsSelector->selectedTags();
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
