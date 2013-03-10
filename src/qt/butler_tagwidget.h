/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGWIDGET_H
#define BUTLER_TAGWIDGET_H

#include <QWidget>
#include <QCheckBox>

#include <butler_tag_set.h>

#include <csjp_reference_container.h>

#include "butler_databases.h"

class QGridLayout;

class TagWidget;

class TagWidget : public QWidget
{
private:
	Q_OBJECT

public:
	TagWidget(const QString & dbname, QWidget *parent);

	void prepareContent();
	void setTags(const TagNameSet &tags);
	void getTags(TagNameSet &tags);
	void selectAll();
	void deselectAll();

private:
	const QString & dbname;
	const TagSet & tagSet;
	csjp::ReferenceContainer<QCheckBox> btnContainer;
	QGridLayout *gridLayout;
};

bool operator<(const QCheckBox &a, const QCheckBox &b);

#endif
