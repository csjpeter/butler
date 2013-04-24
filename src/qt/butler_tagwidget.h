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
	MY_Q_OBJECT

public:
	TagWidget(const QString & dbname, QWidget * parent = 0);

public:
	void setTags(const TagNameSet &tags);
	TagNameSet selectedTags();
	void selectAll();
	void deselectAll();
	virtual QSize sizeHint() const;
	virtual int heightForWidth(int w) const;

	Label label;

signals:
	void selectionChanged();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

private slots:
	void populate();
	void retranslate();
	void applyLayout();
	void relayout();
	void selectionChangedSlot();

private:
	const QString & dbname;
	const TagSet & tagSet;
	csjp::SorterOwnerContainer<QCheckBox> btnContainer;

	int columns;
	int maxTagCheckboxWidth;
};

bool operator<(const QCheckBox &a, const QCheckBox &b);

#endif
