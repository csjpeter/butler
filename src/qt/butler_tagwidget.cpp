/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_tagwidget.h"
#include "butler_waresmodel.h"

TagWidget::TagWidget(const QString & dbname, QWidget *parent) :
	QWidget(parent),
	dbname(dbname),
	tagSet(tagsModel(dbname).tagSet())
{
	prepareContent();
}

void TagWidget::prepareContent()
{
	delete layout();
	gridLayout = new QGridLayout();
	setLayout(gridLayout);

	unsigned i, s = tagSet.size();

	/* Trick:
	 * Add new buttons to a ReferenceContainer in which later we can
	 * access the button belonging to a particular tag. */
	for(i = 0; i < s; i++){
		QCheckBox *tagBox;
		tagBox = new QCheckBox;
		tagBox->setTristate(false);
		btnContainer.add(*tagBox);
	}

	unsigned columns = 4;
	unsigned rows = (s % columns) ? s / columns + 1 : s / columns;

	/* With this the nth tag in tagset will be represented by the nth
	 * button in the btnContainer. */
	for(i = 0; i < s; i++){
		const Tag &tag = tagSet.queryAt(i);
		QCheckBox &tagBox = btnContainer.queryAt(i);
		tagBox.setText(tag.name);

		int col = i / rows;
		int row = i % rows;
		gridLayout->addWidget(&tagBox, row, col);
	}
}

void TagWidget::setTags(const TagNameSet &tags)
{
	if(tagSet.size() != btnContainer.size())
		prepareContent();
	
	unsigned i, s = tagSet.size();
	for(i = 0; i < s; i++){
		const Tag &tag = tagSet.queryAt(i);
		QCheckBox &tagBox = btnContainer.queryAt(i);
		if(tags.has(tag.name))
			tagBox.setChecked(true);
		else
			tagBox.setChecked(false);
	}
}

void TagWidget::getTags(TagNameSet &tags)
{
	if(tagSet.size() != btnContainer.size())
		throw csjp::InvariantFailure("Number of tags in TagSet since "
				"opening the view containing TagWidget changed!\n"
				"You should use this widget only in "
				"top-level dialogs!");

	unsigned i, s = tagSet.size();
	for(i = 0; i < s; i++){
		const Tag &tag = tagSet.queryAt(i);
		QCheckBox &tagBox = btnContainer.queryAt(i);
		if(tagBox.isChecked()){
			if(!tags.has(tag.name))
				tags.add(new QString(tag.name));
		} else {
			if(tags.has(tag.name))
				tags.remove(tag.name);
		}
	}
}

void TagWidget::selectAll()
{
	unsigned i, s = tagSet.size();
	for(i = 0; i < s; i++){
		QCheckBox &tagBox = btnContainer.queryAt(i);
		tagBox.setChecked(true);
	}
}

void TagWidget::deselectAll()
{
	unsigned i, s = tagSet.size();
	for(i = 0; i < s; i++){
		QCheckBox &tagBox = btnContainer.queryAt(i);
		tagBox.setChecked(false);
	}
}

bool operator<(const QCheckBox &a, const QCheckBox &b)
{
	return &a < &b;
}
