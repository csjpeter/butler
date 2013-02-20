/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_tagwidget.h"
#include "butler_waresmodel.h"

namespace Butler {

	TagWidget::TagWidget(QWidget *parent, const TagSet &tagSet) :
		QWidget(parent),
		tagSet(tagSet)
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
		if(tagSet.size() != btnContainer.size()){
			CRITICAL("Number of tags in TagSet since "
					"opening view with TagWidget changed!\n"
					"You should use this widget only in "
					"top-level dialogs!");
			return;
		}

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
}

bool operator<(const QCheckBox &a, const QCheckBox &b)
{
	return &a < &b;
}



