/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_infoview.h"

InfoView::InfoView(QWidget *parent) :
	QWidget(parent),
	textView(0)
{
	QString license(ENDUSER_LICENSE);

	QVBoxLayout * layout = new QVBoxLayout;

	textView = new QPlainTextEdit(license, this);
	textView->setReadOnly(true);
	layout->addWidget(textView);

	QHBoxLayout * hLayout = new QHBoxLayout;

	hLayout->addStretch(0);

	QPushButton * button = new QPushButton(tr("Close"));
	button->setDefault(true);
	connect(button, SIGNAL(clicked()), this, SLOT(close()));
	hLayout->addWidget(button);

	hLayout->addStretch(0);

	layout->addLayout(hLayout);

	/* restore last state */
	loadState();

	setLayout(layout);

	scroll.enableKineticScrollFor(textView);
}

InfoView::~InfoView()
{
	scroll.disableKineticScrollFor(textView);
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
