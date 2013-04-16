/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>
#include "butler_config.h"

#include "butler_infoview.h"

InfoView::InfoView(QWidget *parent) :
	PannView(parent),
	label(0)
{
	setWindowTitle(tr("About this software"));
	setWindowIcon(QIcon(Path::icon("info.png")));
	setWindowModality(Qt::ApplicationModal);

	QString license = QString::fromUtf8(ENDUSER_LICENSE);

	QVBoxLayout * layout = new QVBoxLayout;

	label = new QLabel(license);
	label->setWordWrap(true);
	layout->addWidget(label);

	QHBoxLayout * hLayout = new QHBoxLayout;

	hLayout->addStretch(0);

	QPushButton * button = new QPushButton(tr("Close"));
	connect(button, SIGNAL(clicked()), this, SLOT(accept()));
	hLayout->addWidget(button);

	hLayout->addStretch(0);

	layout->addLayout(hLayout);

	setLayout(layout);
	loadState();
}

InfoView::~InfoView()
{
}

void InfoView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
}

void InfoView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void InfoView::loadState()
{
	QString prefix("InfoView");
	PannView::loadState(prefix);
}

void InfoView::saveState()
{
	QString prefix("InfoView");
	PannView::saveState(prefix);
}
