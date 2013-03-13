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
#include "butler_stockview.h"
#include "butler_tagsview.h"
#include "butler_waresview.h"
#include "butler_customview.h"
#include "butler_queryoptionsview.h"
#include "butler_infoview.h"

#include "butler_application.h"
#include "butler_config.h"

#include "butler_mainview.h"

MainView::MainView(const QString & dbname, QWidget *parent) :
	QWidget(parent),
	dbname(dbname),
	shoppingView(NULL),
	stockView(NULL),
	customView(NULL),
	shopsView(NULL),
	tagsView(NULL),
	waresView(NULL),
	queryOptionsView(NULL),
	infoView(NULL)
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);

	QPushButton * button;

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("shopping.png")), tr("&Shopping"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openShoppingView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("stock.png")), tr("&Maintain stock"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openStockView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("custom.png")), tr("&Custom list"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openCustomView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("shop.png")), tr("S&hops"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openShopsView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("tag.png")), tr("&Tags"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openTagsView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("ware.png")), tr("&Wares"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openWaresView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	button = new QPushButton(QIcon(Path::icon("info.png")), tr("&Info"));
	button->setDefault(false);
	button->setFlat(true);
	connect(button, SIGNAL(clicked()), this, SLOT(openInfoView()));
	layout->addWidget(button, 0, Qt::AlignLeft);

	layout->addStretch(0);

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addStretch(0);
	hLayout->addLayout(layout);
	hLayout->addStretch(0);
	setLayout(hLayout);

	/* restore last state */
	loadState();

	QSettings settings(this);
	if(settings.value("mainview/shoppingview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openShoppingView()));
	if(settings.value("mainview/stockview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openStockView()));
	if(settings.value("mainview/customview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openCustomView()));
	if(settings.value("mainview/shopsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openShopsView()));
	if(settings.value("mainview/tagsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openTagsView()));
	if(settings.value("mainview/waresview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openWaresView()));
	if(settings.value("mainview/queryoptionsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openQueryOptionsView()));
	if(settings.value("mainview/infoview", false).toBool())
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

	if(stockView){
		stockView->deleteLater();
		stockView->close();
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

void MainView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("mainview/position", QPoint()).toPoint();
	QSize size = settings.value("mainview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void MainView::saveState()
{
	QSettings settings(this);
	settings.setValue("mainview/position", pos());
	settings.setValue("mainview/size", size());
	settings.setValue("mainview/shoppingview",
			shoppingView != NULL && shoppingView->isVisible());
	settings.setValue("mainview/stockview",
			stockView != NULL && stockView->isVisible());
	settings.setValue("mainview/customview",
			customView != NULL && customView->isVisible());
	settings.setValue("mainview/shopsview",
			shopsView != NULL && shopsView->isVisible());
	settings.setValue("mainview/tagsview",
			tagsView != NULL && tagsView->isVisible());
	settings.setValue("mainview/waresview",
			waresView != NULL && waresView->isVisible());
	settings.setValue("mainview/queryoptionsview",
			queryOptionsView != NULL && queryOptionsView->isVisible());
	settings.setValue("mainview/infoview",
			infoView != NULL && infoView->isVisible());

	settings.setValue("mainview/dbfile", databaseDescriptor(dbname).databaseName);
}

void MainView::openShoppingView()
{
	if(!shoppingView){
		shoppingView = new ShoppingView(dbname);
		shoppingView->setWindowTitle(tr("Shopping list"));
	}
	shoppingView->show();
	shoppingView->raise();
	shoppingView->activateWindow();
}

void MainView::openStockView()
{
	if(!stockView){
		stockView = new StockView(dbname);
		stockView->setWindowTitle(tr("Stock list"));
	}
	stockView->show();
	stockView->raise();
	stockView->activateWindow();
}

void MainView::openShopsView()
{
	if(!shopsView){
		shopsView = new ShopsView(dbname);
		shopsView->setWindowTitle(tr("Shop list"));
	}
	shopsView->show();
	shopsView->raise();
	shopsView->activateWindow();
}

void MainView::openCustomView()
{
	if(!customView){
		customView = new CustomView(dbname);
		customView->setWindowTitle(tr("User query result"));
	}
	if(!customView->isVisible()){
		customView->show();
		customView->raise();
		customView->activateWindow();
	} else {
		CustomView *anotherCustomView;
		anotherCustomView = new CustomView(dbname, true); /* Deletes itself on close. */
		anotherCustomView->setWindowTitle(tr("User query result"));
		anotherCustomView->show();
		anotherCustomView->raise();
		anotherCustomView->activateWindow();
	}
}

void MainView::openTagsView()
{
	if(!tagsView){
		tagsView = new TagsView(dbname);
		tagsView->setWindowTitle(tr("Tag editor"));
	}
	tagsView->show();
	tagsView->raise();
	tagsView->activateWindow();
}

void MainView::openWaresView()
{
	if(!waresView){
		waresView = new WaresView(dbname);
		waresView->setWindowTitle(tr("Ware editor"));
	}
	waresView->show();
	waresView->raise();
	waresView->activateWindow();
}

void MainView::openQueryOptionsView()
{
#if 0
	if(!queryOptionsView){
	queryOptionsView = new QueryOptionsView();
	queryOptionsView->setWindowTitle(tr("Set query options"));
	}
	queryOptionsView->show();
#endif
}

void MainView::openInfoView()
{
	if(!infoView){
		infoView = new InfoView();
		infoView->setWindowTitle(tr("About this software"));
	}
	infoView->show();
	infoView->raise();
	infoView->activateWindow();
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
