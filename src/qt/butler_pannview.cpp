/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_pannview.h"

PannView::PannView(QWidget *parent) :
	QWidget(parent),
	scrollArea(0)
{
}

PannView::~PannView()
{
	scroll.disableKineticScrollFor(scrollArea);
}

void PannView::setLayout(QLayout * layout)
{
	if(scrollArea){
		scroll.disableKineticScrollFor(scrollArea);
		delete scrollArea;
	}

	scrollArea = new QScrollArea(this);
	scrollArea->setFrameStyle(QFrame::NoFrame);
	QWidget * main = new QWidget(scrollArea);
	main->setLayout(layout);

	scrollArea->setWidget(main);
	scrollArea->setWidgetResizable(true);
//	scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(scrollArea);
	vLayout->setContentsMargins(0,0,0,0);
	QWidget::setLayout(vLayout);

	scroll.enableKineticScrollFor(scrollArea);
}
