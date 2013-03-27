/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_pannview.h"

PannView::PannView(QWidget *parent) :
	QWidget(parent),
	scrollArea(0),
	scroll(0)
{
}

PannView::~PannView()
{
}

void PannView::setLayout(QLayout * layout)
{
	if(scrollArea){
		scroll.disableKineticScroll();
		delete scrollArea;
	}

	QWidget * main = new QWidget;
	main->setLayout(layout);

	scrollArea = new QScrollArea;
	scrollArea->setFrameStyle(QFrame::NoFrame);
	scrollArea->setWidget(main);
	scrollArea->setWidgetResizable(true);
//	scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(scrollArea);
	vLayout->setContentsMargins(0,0,0,0);
	QWidget::setLayout(vLayout);

	scroll.enableKineticScrollFor(scrollArea);
}
