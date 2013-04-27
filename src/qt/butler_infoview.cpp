/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>
#include "butler_config.h"

#include "butler_infoview.h"

//SCC TidContext = "InfoView";

SCC TidInfoWindowTitle = QT_TRANSLATE_NOOP("InfoView", "License informations");

InfoView::InfoView(QWidget *parent) :
	PannView(parent),
	toolBar(this)
{
	setWindowTitle(trLong(TidInfoWindowTitle));
	setWindowIcon(QIcon(Path::icon("info.png")));
	setWindowModality(Qt::ApplicationModal);

	setToolBar(&toolBar);

	QString license = QString::fromUtf8(ENDUSER_LICENSE);

	QVBoxLayout * layout = new QVBoxLayout;

	label.setText(license);
	label.setWordWrap(true);
	layout->addWidget(&label);

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
