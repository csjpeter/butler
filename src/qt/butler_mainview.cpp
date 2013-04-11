/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include <butler_db.h>

#include "butler_databases.h"

#include "butler_shoppingview.h"
#include "butler_shopsview.h"
#include "butler_tagsview.h"
#include "butler_waresview.h"
#include "butler_customview.h"
#include "butler_queryoptionsview.h"
#include "butler_infoview.h"

#include "butler_config.h"

#include "butler_mainview.h"

MainView::MainView(const QString & dbname, QWidget *parent) :
	PannView(parent),
	dbname(dbname),
	shortcut(QKeySequence(Qt::ALT + Qt::Key_Escape), this),
	shoppingView(NULL),
	shopsView(NULL),
	tagsView(NULL),
	waresView(NULL),
	customView(NULL),
	queryOptionsView(NULL),
	infoView(NULL)
{
	setWindowTitle(QString(PKGNAME) + " - " + tr("Main view"));
	connect(&shortcut, SIGNAL(activated()), this, SLOT(reject()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	layout->setSpacing(0);

	QPushButton * button;

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("shopping.png")), tr("&Shopping"));
	connect(button, SIGNAL(clicked()), this, SLOT(openShoppingView()));
	layout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("list.png")), tr("&List items"));
	connect(button, SIGNAL(clicked()), this, SLOT(openCustomView()));
	layout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("partner.png")), tr("&Partners"));
	connect(button, SIGNAL(clicked()), this, SLOT(openShopsView()));
	layout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("ware.png")), tr("&Wares"));
	connect(button, SIGNAL(clicked()), this, SLOT(openWaresView()));
	layout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("tag.png")), tr("&Tags"));
	connect(button, SIGNAL(clicked()), this, SLOT(openTagsView()));
	layout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("info.png")), tr("&Info"));
	connect(button, SIGNAL(clicked()), this, SLOT(openInfoView()));
	layout->addWidget(button);

	layout->addStretch(0);

	loadState();
	setLayout(layout);

	QSettings settings;
	QString className = metaObject()->className();
	if(settings.value(className + "/shoppingview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openShoppingView()));
	if(settings.value(className + "/customview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openCustomView()));
	if(settings.value(className + "/shopsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openShopsView()));
	if(settings.value(className + "/tagsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openTagsView()));
	if(settings.value(className + "/waresview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openWaresView()));
	if(settings.value(className + "/queryoptionsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openQueryOptionsView()));
	if(settings.value(className + "/infoview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openInfoView()));
}

MainView::~MainView()
{
}

void MainView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
}

void MainView::closeEvent(QCloseEvent *event)
{
	saveState();

	if(shoppingView){
		shoppingView->deleteLater();
		shoppingView->close();
	}

	if(customView){
		customView->deleteLater();
		customView->close();
	}

	if(shopsView){
		shopsView->deleteLater();
		shopsView->close();
	}

	if(tagsView){
		tagsView->deleteLater();
		tagsView->close();
	}

	if(waresView){
		waresView->deleteLater();
		waresView->close();
	}

	if(queryOptionsView){
		queryOptionsView->deleteLater();
		queryOptionsView->close();
	}

	QTimer::singleShot(0, this, SLOT(quit()));

	QWidget::closeEvent(event);
}

void MainView::saveState()
{
	PannView::saveState();
	QString className = metaObject()->className();

	QSettings settings;
	settings.setValue(className + "/shoppingview",
			shoppingView != NULL && shoppingView->isVisible());
	settings.setValue(className + "/customview",
			customView != NULL && customView->isVisible());
	settings.setValue(className + "/shopsview",
			shopsView != NULL && shopsView->isVisible());
	settings.setValue(className + "/tagsview",
			tagsView != NULL && tagsView->isVisible());
	settings.setValue(className + "/waresview",
			waresView != NULL && waresView->isVisible());
	settings.setValue(className + "/queryoptionsview",
			queryOptionsView != NULL && queryOptionsView->isVisible());
	settings.setValue(className + "/infoview",
			infoView != NULL && infoView->isVisible());

	settings.setValue(className + "/dbfile", databaseDescriptor(dbname).databaseName);
}

void MainView::openShoppingView()
{
	if(!shoppingView)
		shoppingView = new ShoppingView(dbname);
	shoppingView->activate();
}

void MainView::openShopsView()
{
	if(!shopsView)
		shopsView = new ShopsView(dbname);
	shopsView->activate();
}

void MainView::openCustomView()
{
	if(!customView)
		customView = new CustomView(dbname);

	if(!customView->isVisible()){
		customView->activate();
	} else {
		CustomView *anotherCustomView;
		anotherCustomView = new CustomView(dbname, true); /* Deletes itself on close. */
		anotherCustomView->setWindowTitle(tr("User query result"));
		anotherCustomView->activate();
	}
}

void MainView::openTagsView()
{
	if(!tagsView)
		tagsView = new TagsView(dbname);
	tagsView->activate();
}

void MainView::openWaresView()
{
	if(!waresView)
		waresView = new WaresView(dbname);
	waresView->activate();
}

void MainView::openQueryOptionsView()
{
#if 0
	if(!queryOptionsView)
		queryOptionsView = new QueryOptionsView;
	queryOptionsView->activate();
#endif
}

void MainView::openInfoView()
{
	if(!infoView)
		infoView = new InfoView;
	infoView->activate();
}

void MainView::about()
{
	QMessageBox::about(this, tr("Butler"),
			tr("Butler is a helper for home keeping via maintaining shoping lists."));
}

void MainView::quit()
{
	QCoreApplication::quit();
}
