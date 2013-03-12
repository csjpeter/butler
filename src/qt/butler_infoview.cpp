/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_infoview.h"

InfoView::InfoView(QWidget *parent) :
	QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout;

	QPlainTextEdit * textView = new QPlainTextEdit(this);
	textView->setReadOnly(true);
	layout->addWidget(textView);

	QPushButton * button = new QPushButton(tr("Close"));
	button->setDefault(true);
	connect(button, SIGNAL(clicked()), this, SLOT(close()));
	layout->addWidget(button);

	/* restore last state */
	loadState();

	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
}

InfoView::~InfoView()
{
}

void InfoView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
}

void InfoView::closeEvent(QCloseEvent *event)
{
	saveState();
	QWidget::closeEvent(event);
}

void InfoView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("infoview/position", QPoint()).toPoint();
	QSize size = settings.value("infoview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void InfoView::saveState()
{
	QSettings settings(this);
	settings.setValue("infoview/position", pos());
	settings.setValue("infoview/size", size());
}
