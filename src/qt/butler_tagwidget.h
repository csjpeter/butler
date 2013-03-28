/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGWIDGET_H
#define BUTLER_TAGWIDGET_H

#include <QtGui>

#include <butler_tag_set.h>
#include <csjp_sorter_reference_container.h>

#include "butler_databases.h"

class TagWidget : public QWidget
{
private:
	Q_OBJECT

public:
	TagWidget(const QString & dbname, QWidget * parent = 0);

public slots:
	void relayout();

public:
	void setTags(const TagNameSet &tags);
	void getTags(TagNameSet &tags);
	void selectAll();
	void deselectAll();

public:
	void resizeEvent(QResizeEvent *event);

private:
	const QString & dbname;
	const TagSet & tagSet;
	csjp::SorterReferenceContainer<QCheckBox> btnContainer;
	QGridLayout *gridLayout;
	int maxTagCheckboxWidth;
};

bool operator<(const QCheckBox &a, const QCheckBox &b);

#endif
