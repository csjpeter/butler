/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_infoview.h"

InfoView::InfoView(QWidget *parent) :
	PannView(parent),
	label(0)
{
	setWindowTitle(tr("About this software"));
	setWindowModality(Qt::ApplicationModal);

	QString license = QString::fromUtf8(ENDUSER_LICENSE);

	QVBoxLayout * layout = new QVBoxLayout;

	label = new QLabel(license);
	label->setWordWrap(true);
	layout->addWidget(label);

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
	QSettings settings;
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
	QSettings settings;
	settings.setValue("infoview/position", pos());
	settings.setValue("infoview/size", size());
}
