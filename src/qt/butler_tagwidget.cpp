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
	columns(1)
{
	relayout();
}

void TagWidget::applyLayout()
{
	delete layout();

	VLayout *mainLayout = new VLayout;

	QGridLayout * gridLayout = new QGridLayout;
	gridLayout->setContentsMargins(0,0,0,0);

	unsigned i, s = tagSet.size();
	unsigned rows = (s % columns) ? s / columns + 1 : s / columns;

	/* With this the nth tag in tagset will be represented by the nth
	 * button in the btnContainer. */
	for(i = 0; i < s; i++){
		QCheckBox & tagBox = btnContainer.queryAt(i);

		int col = i / rows;
		int row = i % rows;
		gridLayout->addWidget(&tagBox, row, col);
	}

	mainLayout->addWidget(&label);
	mainLayout->addLayout(gridLayout);

	setLayout(mainLayout);
}

void TagWidget::selectionChangedSlot()
{
	selectionChanged();
}

void TagWidget::relayout()
{
	bool changed = false;
	unsigned i, s = tagSet.size();

	/* Trick:
	 * Add new buttons to btnContainer in which later we can
	 * access the button belonging to a particular tag. */
	for(i = btnContainer.size(); i < s; i++){
		QCheckBox * tagBox(new QCheckBox());
		tagBox->setContentsMargins(0,0,0,0);
		tagBox->setTristate(false);
		connect(tagBox, SIGNAL(stateChanged(int)), this, SLOT(selectionChangedSlot()));
		btnContainer.add(tagBox);
		changed = true;
	}

	int maxTagCheckboxWidth = 1;
	for(i = 0; i < s; i++){
		QCheckBox & tagBox = btnContainer.queryAt(i);
		const Tag &tag = tagSet.queryAt(i);

		if(tagBox.text() != tag.name){
			tagBox.setText(tag.name);
			changed = true;
		}

		/* For guessing practical number of columns. */
		int w = tagBox.sizeHint().width();
		if(maxTagCheckboxWidth < w)
			maxTagCheckboxWidth = w;
	}

	int newColumns = width() / maxTagCheckboxWidth;
	if(!newColumns)
		newColumns = 1;

	if(newColumns != columns || changed)
		applyLayout();
}

void TagWidget::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
	if(tagSet.size() == btnContainer.size())
		return;
	relayout();
}

void TagWidget::resizeEvent(QResizeEvent *event)
{
	if(layout() && (event->size() == event->oldSize() || !isVisible()))
		return;
	relayout();
}

void TagWidget::setTags(const TagNameSet &tags)
{
	if(tagSet.size() != btnContainer.size())
		relayout();
	
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

TagNameSet && TagWidget::selectedTags()
{
	if(tagSet.size() != btnContainer.size())
		throw csjp::InvariantFailure("Number of tags in TagSet since "
				"opening the view containing TagWidget changed!\n"
				"You should use this widget only in "
				"top-level dialogs!");

	TagNameSet tags;
	unsigned i, s = btnContainer.size();
	for(i = 0; i < s; i++){
		QCheckBox & tagBox = btnContainer.queryAt(i);
		if(tagBox.isChecked())
			if(tagSet.has(tagBox.text())) // a tag might had been removed
				tags.add(new QString(tagBox.text()));
	}
	return csjp::move_cast(tags);
}

void TagWidget::selectAll()
{
	unsigned i, s = btnContainer.size();
	for(i = 0; i < s; i++){
		QCheckBox &tagBox = btnContainer.queryAt(i);
		tagBox.setChecked(true);
	}
}

void TagWidget::deselectAll()
{
	unsigned i, s = btnContainer.size();
	for(i = 0; i < s; i++){
		QCheckBox &tagBox = btnContainer.queryAt(i);
		tagBox.setChecked(false);
	}
}

bool operator<(const QCheckBox &a, const QCheckBox &b)
{
	return &a < &b;
}
