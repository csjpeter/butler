/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGWIDGET_H
#define BUTLER_TAGWIDGET_H

#include <QtGui>

#include <csjp_sorter_owner_container.h>

#include <butler_dataclasses.h>
#include <butler_widgets.h>

#include "butler_databases.h"

class TagWidget : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	TagWidget(const csjp::String & dbname, QWidget * parent = 0);

public:
	void setTags(const QueryWithTagSet & tags);
	void setTags(const QueryWithoutTagSet & tags);
	void setTags(const WareTagSet & tags);
	StringSet selectedTags();
	void selectAll();
	void deselectAll();
	virtual QSize sizeHint() const;
	virtual int heightForWidth(int w) const;

	Label label;

signals:
	void selectionChanged();

private:
	void setTags(const StringSet & tags);
	virtual void showEvent(QShowEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

private slots:
	void populate();
	void retranslate();
	void applyLayout();
	void relayout();
	void selectionChangedSlot();

private:
	const csjp::String & dbname;
	const TagSet & tagSet;
	csjp::SorterOwnerContainer<QCheckBox> btnContainer;

	int columns;
	int maxTagCheckboxWidth;
};

bool operator<(const QCheckBox & a, const QCheckBox & b);

#endif
