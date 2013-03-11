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

#include "butler_application.h"

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
	queryOptionsView(NULL)
{
	/* Quit when main window (this) is closed. */
	setAttribute(Qt::WA_QuitOnClose, true);

	/* action toolbar */
	actionTB = new QToolBar(tr("Action toolbar"));

	/* actions */
	shoppingAct = new QAction(QIcon(ICONS_PATH "shopping.png"), tr("&Shopping"), this);
	shoppingAct->setShortcut(tr("S"));
	shoppingAct->setToolTip(tr("Shopping list"));
	connect(shoppingAct, SIGNAL(triggered()), this, SLOT(openShoppingView()));
	shoppingTBtn = new QToolButton(actionTB);
	shoppingTBtn->setDefaultAction(shoppingAct);
	shoppingTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	actionTB->addWidget(shoppingTBtn);

	stockAct = new QAction(QIcon(ICONS_PATH "stock.png"), tr("&Maintain stock"), this);
	stockAct->setShortcut(tr("M"));
	stockAct->setToolTip(tr("Maintain stock list"));
	connect(stockAct, SIGNAL(triggered()), this, SLOT(openStockView()));
	stockTBtn = new QToolButton(actionTB);
	stockTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	stockTBtn->setDefaultAction(stockAct);
	actionTB->addWidget(stockTBtn);

	customAct = new QAction(QIcon(ICONS_PATH "custom.png"), tr("&Custom list"), this);
	customAct->setShortcut(tr("C"));
	customAct->setToolTip(tr("Lists by custom queries"));
	connect(customAct, SIGNAL(triggered()), this, SLOT(openCustomView()));
	customTBtn = new QToolButton(actionTB);
	customTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	customTBtn->setDefaultAction(customAct);
	actionTB->addWidget(customTBtn);

	shopsAct = new QAction(QIcon(ICONS_PATH "shop.png"), tr("S&hops"), this);
	shopsAct->setShortcut(tr("L"));
	shopsAct->setToolTip(tr("List of shops"));
	connect(shopsAct, SIGNAL(triggered()), this, SLOT(openShopsView()));
	shopsTBtn = new QToolButton(actionTB);
	shopsTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	shopsTBtn->setDefaultAction(shopsAct);
	actionTB->addWidget(shopsTBtn);

	tagsAct = new QAction(QIcon(ICONS_PATH "tag.png"), tr("&Tags"), this);
	tagsAct->setShortcut(tr("T"));
	tagsAct->setToolTip(tr("Tag editor"));
	connect(tagsAct, SIGNAL(triggered()), this, SLOT(openTagsView()));
	tagsTBtn = new QToolButton(actionTB);
	tagsTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	tagsTBtn->setDefaultAction(tagsAct);
	actionTB->addWidget(tagsTBtn);

	waresAct = new QAction(QIcon(ICONS_PATH "ware.png"), tr("&Wares"), this);
	waresAct->setShortcut(tr("W"));
	waresAct->setToolTip(tr("Ware editor"));
	connect(waresAct, SIGNAL(triggered()), this, SLOT(openWaresView()));
	waresTBtn = new QToolButton(actionTB);
	waresTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	waresTBtn->setDefaultAction(waresAct);
	actionTB->addWidget(waresTBtn);

	/*		queryOptionsAct = new QAction(QIcon(ICONS_PATH "query.png"), tr("&Queries"), this);
			queryOptionsAct->setShortcut(tr("Q"));
			queryOptionsAct->setToolTip(tr("Set filter options for user queries"));
			connect(queryOptionsAct, SIGNAL(triggered()), this, SLOT(openQueryOptionsView()));
			queryOptionsTBtn = new QToolButton(actionTB);
			queryOptionsTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
			queryOptionsTBtn->setDefaultAction(queryOptionsAct);
			actionTB->addWidget(queryOptionsTBtn);
			*/
	/* making the window layouting */
	//		QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
	//		actionTB->setOrientation(Qt::Vertical);
	actionTB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	//		actionTB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	actionTB->setIconSize(QSize(64,64));
	layout->addWidget(actionTB);

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

void MainView::about()
{
	QMessageBox::about(this, tr("Butler"),
			tr("Butler is a helper for home keeping via maintaining shoping lists."));
}

void MainView::quit()
{
	QCoreApplication::quit();
}
