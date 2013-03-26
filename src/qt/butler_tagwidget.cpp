/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_tagwidget.h"
#include "butler_waresmodel.h"

TagWidget::TagWidget(const QString & dbname, QWidget * parent) :
	QWidget(parent),
	dbname(dbname),
	tagSet(tagsModel(dbname).tagSet()),
	gridLayout(0),
	maxTagCheckboxWidth(50)
{
	prepareContent();
}

void TagWidget::prepareContent()
{
	/* Guessing practical number of columns. */
	int columns = 1;
	if(parentWidget())
		columns = parentWidget()->width() / maxTagCheckboxWidth;
	if(!columns)
		columns = 1;

	if(gridLayout){
		if(gridLayout->columnCount() == columns)
			return; /* No sense to relayout. */
		QLayoutItem * child;
		while ((child = gridLayout->takeAt(0)) != 0) {
//			child->widget()->deleteLater();
			LOG("Parent: %p", child->widget()->parent());
			delete child;
		}
		delete layout();
		gridLayout = 0;
	}
//	btnContainer.clear();

	unsigned i, s = tagSet.size();

	/* Trick:
	 * Add new buttons to a ReferenceContainer in which later we can
	 * access the button belonging to a particular tag. */
	if(btnContainer.empty()){
		for(i = 0; i < s; i++){
			QCheckBox * tagBox(new QCheckBox());
			tagBox->setTristate(false);

			const Tag &tag = tagSet.queryAt(i);
			tagBox->setText(tag.name);
			int w = tagBox->minimumSizeHint().width();
			if(maxTagCheckboxWidth < w)
				maxTagCheckboxWidth = w;

			btnContainer.add(*tagBox);
		}
	}

	gridLayout = new QGridLayout;
	setLayout(gridLayout);

	unsigned rows = (s % columns) ? s / columns + 1 : s / columns;

	/* With this the nth tag in tagset will be represented by the nth
	 * button in the btnContainer. */
	for(i = 0; i < s; i++){
		QCheckBox &tagBox = btnContainer.queryAt(i);

		int col = i / rows;
		int row = i % rows;
		gridLayout->addWidget(&tagBox, row, col);
	}

	updateGeometry();
}

void TagWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	if(event->size() != event->oldSize())
		QTimer::singleShot(0, this, SLOT(prepareContent()));
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
