/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

#include "butler_waresmodel.h"
#include "butler_tagsmodel.h"

class QGridLayout;

namespace Butler {

class TagWidget;

class TagWidget : public QWidget
{
private:
	Q_OBJECT

public:
	TagWidget(QWidget *parent, const TagSet &tagSet);

	void prepareContent();
	void setTags(const TagNameSet &tags);
	void getTags(TagNameSet &tags);
	void selectAll();
	void deselectAll();

private:
	const TagSet &tagSet;
	csjp::ReferenceContainer<QCheckBox> btnContainer;
	QGridLayout *gridLayout;
};

}

bool operator<(const QCheckBox &a, const QCheckBox &b);

#endif


