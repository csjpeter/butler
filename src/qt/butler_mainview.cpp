/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include <butler_db.h>

#include "butler_databases.h"

#include "butler_shoppingview.h"
#include "butler_accountingview.h"
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
	model(customModel(dbname)),
	todoButton(QIcon(Path::icon("list.png")), QKeySequence(Qt::ALT + Qt::Key_F1)),
	shoppingButton(QIcon(Path::icon("shop.png")), QKeySequence(Qt::ALT + Qt::Key_F2)),
	accountingButton(QIcon(Path::icon("accounting.png")), QKeySequence(Qt::ALT + Qt::Key_F3)),
	analiticsButton(QIcon(Path::icon("analitics.png")), QKeySequence(Qt::ALT + Qt::Key_F4)),
	partnersButton(QIcon(Path::icon("partner.png")), QKeySequence(Qt::ALT + Qt::Key_F5)),
	wareButton(QIcon(Path::icon("ware.png")), QKeySequence(Qt::ALT + Qt::Key_F6)),
	tagButton(QIcon(Path::icon("tag.png")), QKeySequence(Qt::ALT + Qt::Key_F7)),
	infoButton(QIcon(Path::icon("info.png")), QKeySequence(Qt::ALT + Qt::Key_F8)),
	quitButton(QIcon(Path::icon("delete.png")), QKeySequence(Qt::ALT + Qt::Key_F9)),
	shoppingView(NULL),
	accountingView(NULL),
	customView(NULL),
	shopsView(NULL),
	waresView(NULL),
	tagsView(NULL),
	queryOptionsView(NULL),
	infoView(NULL)
{
	setWindowIcon(QIcon(Path::icon("list.png")));

//	connect(&todoButton, SIGNAL(clicked()), this, SLOT(openTodoView()));
	connect(&shoppingButton, SIGNAL(clicked()), this, SLOT(openShoppingView()));
	connect(&accountingButton, SIGNAL(clicked()), this, SLOT(openAccountingView()));
	connect(&analiticsButton, SIGNAL(clicked()), this, SLOT(openCustomView()));
	connect(&partnersButton, SIGNAL(clicked()), this, SLOT(openShopsView()));
	connect(&wareButton, SIGNAL(clicked()), this, SLOT(openWaresView()));
	connect(&tagButton, SIGNAL(clicked()), this, SLOT(openTagsView()));
	connect(&infoButton, SIGNAL(clicked()), this, SLOT(openInfoView()));
	connect(&quitButton, SIGNAL(clicked()), this, SLOT(accept()));

	retranslate();
}

MainView::~MainView()
{
//	delete todoView;
	delete shoppingView;
	delete accountingView;
	delete customView;
	delete shopsView;
	delete tagsView;
	delete waresView;
	delete queryOptionsView;
	delete infoView;
}

void MainView::retranslate()
{
	setWindowTitle(qtTrId(TidMainWindowTitle).arg(PKGNAME));
	todoButton.setText(qtTrId(TidTodoButtonLabel));
	shoppingButton.setText(qtTrId(TidShoppingButtonLabel));
	accountingButton.setText(qtTrId(TidAccountingButtonLabel));
	analiticsButton.setText(qtTrId(TidAnaliticsButtonLabel));
	partnersButton.setText(qtTrId(TidPartnersButtonLabel));
	wareButton.setText(qtTrId(TidWareButtonLabel));
	tagButton.setText(qtTrId(TidTagButtonLabel));
	infoButton.setText(qtTrId(TidInfoButtonLabel));
	quitButton.setText(qtTrId(TidQuitButtonLabel));

	relayout();
}

void MainView::applyLayout()
{
	HLayout * mainLayout = new HLayout;

	VLayout * layout = new VLayout;

	layout->addStretch(0);

//	layout->addWidget(&todoButton);
	layout->addWidget(&shoppingButton);
	layout->addWidget(&accountingButton);
	layout->addWidget(&analiticsButton);
	layout->addWidget(&partnersButton);
	layout->addWidget(&wareButton);
	layout->addWidget(&tagButton);
	layout->addWidget(&infoButton);
	layout->addWidget(&quitButton);

	layout->addStretch(0);

	mainLayout->addStretch(0);
	mainLayout->addLayout(layout);
	mainLayout->addStretch(0);

	setLayout(mainLayout);
}

void MainView::relayout()
{
	todoButton.expandingLayout();
	shoppingButton.expandingLayout();
	accountingButton.expandingLayout();
	analiticsButton.expandingLayout();
	partnersButton.expandingLayout();
	wareButton.expandingLayout();
	tagButton.expandingLayout();
	infoButton.expandingLayout();
	quitButton.expandingLayout();

	applyLayout();
}

void MainView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void MainView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void MainView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
}

void MainView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
	QTimer::singleShot(0, qApp, SLOT(quit()));
}

void MainView::loadState()
{
	QString prefix("MainView");
	PannView::loadState(prefix);
	QSettings settings;

	if(settings.value(prefix + "/shoppingview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openShoppingView()));
	if(settings.value(prefix + "/customview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openCustomView()));
	if(settings.value(prefix + "/shopsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openShopsView()));
	if(settings.value(prefix + "/tagsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openTagsView()));
	if(settings.value(prefix + "/waresview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openWaresView()));
	if(settings.value(prefix + "/queryoptionsview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openQueryOptionsView()));
	if(settings.value(prefix + "/infoview", false).toBool())
		QTimer::singleShot(0, this, SLOT(openInfoView()));
}

void MainView::saveState()
{
	QString prefix("MainView");
	PannView::saveState(prefix);
	QSettings settings;

	settings.setValue(prefix + "/shoppingview",
			shoppingView != NULL && shoppingView->isVisible());
	settings.setValue(prefix + "/customview",
			customView != NULL && customView->isVisible());
	settings.setValue(prefix + "/shopsview",
			shopsView != NULL && shopsView->isVisible());
	settings.setValue(prefix + "/tagsview",
			tagsView != NULL && tagsView->isVisible());
	settings.setValue(prefix + "/waresview",
			waresView != NULL && waresView->isVisible());
	settings.setValue(prefix + "/queryoptionsview",
			queryOptionsView != NULL && queryOptionsView->isVisible());
	settings.setValue(prefix + "/infoview",
			infoView != NULL && infoView->isVisible());

	settings.setValue(prefix + "/dbfile", databaseDescriptor(dbname).databaseName);
}

void MainView::openShoppingView()
{
	if(!shoppingView)
		shoppingView = new ShoppingView(dbname);
	shoppingView->activate();
}

void MainView::openAccountingView()
{
	if(!accountingView)
		accountingView = new AccountingView(dbname, *model);
	accountingView->activate();
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

void MainView::openShopsView()
{
	if(!shopsView)
		shopsView = new ShopsView(dbname);
	shopsView->activate();
}

void MainView::openWaresView()
{
	if(!waresView)
		waresView = new WaresView(dbname);
	waresView->activate();
}

void MainView::openTagsView()
{
	if(!tagsView)
		tagsView = new TagsView(dbname);
	tagsView->activate();
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
