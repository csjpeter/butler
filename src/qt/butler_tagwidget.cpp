/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG 1

#include <QtGui>

#include "butler_config.h"
#include "butler_tagwidget.h"
#include "butler_waresmodel.h"


TagWidget::TagWidget(const QString & dbname, QWidget * parent) :
	QWidget(parent),
	dbname(dbname),
	tagSet(tagsModel(dbname).tagSet()),
	columns(1),
	maxTagCheckboxWidth(1)
{
	populate();
}

void TagWidget::retranslate()
{
}

void TagWidget::applyLayout()
{
	delete layout();

	VLayout *mainLayout = new VLayout;
	mainLayout->setSpacing(0);

	QGridLayout * gridLayout = new QGridLayout;
	gridLayout->setContentsMargins(0,0,0,0);

	unsigned i, s = tagSet.size();
	unsigned rows = (s % columns) ? s / columns + 1 : s / columns;
	DBG("Rows to apply %d", rows);

	/* With this the nth tag in tagset will be represented by the nth
	 * button in the btnContainer. */
	for(i = 0; i < s; i++){
		QCheckBox & tagBox = btnContainer.queryAt(i);

		int col = i / rows;
		int row = i % rows;
		gridLayout->addWidget(&tagBox, row, col);
	}

	if(label.text().size())
		mainLayout->addWidget(&label);
	mainLayout->addLayout(gridLayout);

	setLayout(mainLayout);
	updateGeometry();
}

void TagWidget::relayout()
{
	DBG("Width: %d TagLabelWidth: %d", width(), maxTagCheckboxWidth);
	int newColumns = width() / maxTagCheckboxWidth;
	if(!newColumns)
		newColumns = 1;

	if(newColumns != columns){
		columns = newColumns;
		DBG("Columns changed to %d", columns);
		applyLayout();
	}
}

QSize TagWidget::sizeHint() const
{
	int w = maxTagCheckboxWidth;
	int lw = label.sizeHint().width();
	if(w < lw)
		w = lw;
	DBG("SizeHint: %d, %d", w, -1);
	return QSize(w, -1);
}

int TagWidget::heightForWidth(int w) const
{
	int h = label.sizeHint().height();
	int cols = w / maxTagCheckboxWidth;
	if(!cols)
		cols = 1;
	int rows = btnContainer.size() / cols;
	if((unsigned)(rows * cols) < btnContainer.size())
		rows++;
	if(rows){
		QCheckBox & tagBox = btnContainer.queryAt(0);
		h += rows * tagBox.sizeHint().height();
	}
	DBG("Height: %d For width: %d", h, w);
	return h;
}

void TagWidget::selectionChangedSlot()
{
	selectionChanged();
}

void TagWidget::populate()
{
	bool changed = false;

	/* Trick:
	 * Manage buttons to btnContainer in which later we can
	 * access the button belonging to a particular tag. */

	/* Removing buttons if tags were removed. */
	for(unsigned i = tagSet.size(); i < btnContainer.size(); i++){
		btnContainer.removeAt(i);
		changed = true;
	}

	/* Adding buttons if there are new tags. */
	for(unsigned i = btnContainer.size(); i < tagSet.size(); i++){
		QCheckBox * tagBox(new QCheckBox());
		tagBox->setContentsMargins(0,0,0,0);
		tagBox->setTristate(false);
		connect(tagBox, SIGNAL(stateChanged(int)), this, SLOT(selectionChangedSlot()));
		btnContainer.add(tagBox);
		changed = true;
	}

	/* Update texts */
	for(unsigned i = 0; i < tagSet.size(); i++){
		QCheckBox & tagBox = btnContainer.queryAt(i);
		const Tag & tag = tagSet.queryAt(i);
		if(tagBox.text() != tag.name){
			tagBox.setText(tag.name);
			changed = true;
		}
	}
	DBG("Populate");

	if(changed){
		DBG("Population changed");
		/* Speculating appropriate column width */
		maxTagCheckboxWidth = 1;
		for(unsigned i = 0; i < tagSet.size(); i++){
			QCheckBox & tagBox = btnContainer.queryAt(i);
			/* For guessing practical number of columns. */
			int w = tagBox.sizeHint().width();
			if(maxTagCheckboxWidth < w)
				maxTagCheckboxWidth = w;
		}

		applyLayout();
		//relayout();
	}
}

void TagWidget::showEvent(QShowEvent *event)
{
	populate();
	QWidget::showEvent(event);
}

void TagWidget::resizeEvent(QResizeEvent *event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}

void TagWidget::setTags(const StringSet & tags)
{
	populate();
	
	unsigned i, s = tagSet.size();
	for(i = 0; i < s; i++){
		const Tag & tag = tagSet.queryAt(i);
		QCheckBox & tagBox = btnContainer.queryAt(i);
		if(tags.has(tag.name))
			tagBox.setChecked(true);
		else
			tagBox.setChecked(false);
	}
}

StringSet TagWidget::selectedTags()
{
	if(tagSet.size() != btnContainer.size())
		populate();

	StringSet tags;
	for(QCheckBox & tagBox : btnContainer){
		if(tagBox.isChecked()){
			Text text(tagBox.text());
			if(tagSet.has(text)) // a tag might had been removed
				tags.add(new Text(text));
		}
	}
	return tags;
}

void TagWidget::selectAll()
{
	for(QCheckBox & tagBox : btnContainer)
		tagBox.setChecked(true);
}

void TagWidget::deselectAll()
{
	for(QCheckBox & tagBox : btnContainer)
		tagBox.setChecked(false);
}

bool operator<(const QCheckBox & a, const QCheckBox & b)
{
	return &a < &b;
}
