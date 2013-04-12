/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGWIDGET_H
#define BUTLER_TAGWIDGET_H

#include <QtGui>

#include <csjp_sorter_owner_container.h>

#include <butler_tag_set.h>
#include <butler_widgets.h>

#include "butler_databases.h"

class TagWidget : public QWidget
{
private:
	Q_OBJECT

public:
	TagWidget(const QString & dbname, QWidget * parent = 0);

public:
	void setTags(const TagNameSet &tags);
	TagNameSet && selectedTags();
	void selectAll();
	void deselectAll();

	Label label;

signals:
	void selectionChanged();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

private slots:
	void applyLayout();
	void relayout();
	void selectionChangedSlot();

private:
	const QString & dbname;
	const TagSet & tagSet;
	csjp::SorterOwnerContainer<QCheckBox> btnContainer;

	int columns;
};

bool operator<(const QCheckBox &a, const QCheckBox &b);

#endif
