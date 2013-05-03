/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include <butler_db.h>

#include "butler_databases.h"

#include "butler_shoppingview.h"
#include "butler_edititemview.h"
#include "butler_partnersview.h"
#include "butler_companyview.h"
#include "butler_tagsview.h"
#include "butler_waresview.h"
#include "butler_customview.h"
#include "butler_queryoptionsview.h"
#include "butler_infoview.h"

#include "butler_config.h"

#include "butler_mainview.h"

SCC TidContext = "MainView";

SCC TidMainWindowTitle = QT_TRANSLATE_NOOP("MainView", "%1 - main view");
SCC TidTodoButton = QT_TRANSLATE_NOOP("MainView", "Todo notes");
SCC TidShoppingButton = QT_TRANSLATE_NOOP("MainView", "Shopping list");
SCC TidNewItemButton = QT_TRANSLATE_NOOP("MainView", "Expense/Income");
SCC TidAnaliticsButton = QT_TRANSLATE_NOOP("MainView", "Analitics / History");
SCC TidPartnersButton = QT_TRANSLATE_NOOP("MainView", "Partners");
SCC TidCompanyButton = QT_TRANSLATE_NOOP("MainView", "Companies");
SCC TidWareButton = QT_TRANSLATE_NOOP("MainView", "Wares");
SCC TidTagButton = QT_TRANSLATE_NOOP("MainView", "Tags");
SCC TidInfoButton = QT_TRANSLATE_NOOP("MainView", "Legal informations");
SCC TidQuitButton = QT_TRANSLATE_NOOP("MainView", "Quit");

MainView::MainView(const QString & dbname, QWidget *parent) :
	PannView(parent),
	dbname(dbname),
	model(customModel(dbname)),
	todoButton(QIcon(Path::icon("list.png")),
			TidTodoButton, TidContext, QKeySequence(Qt::Key_F1)),
	shoppingButton(QIcon(Path::icon("shopping.png")),
			TidShoppingButton, TidContext, QKeySequence(Qt::Key_F2)),
	newItemButton(QIcon(Path::icon("accounting.png")),
			TidNewItemButton, TidContext, QKeySequence(Qt::Key_F3)),
	analiticsButton(QIcon(Path::icon("analitics.png")),
			TidAnaliticsButton, TidContext, QKeySequence(Qt::Key_F4)),
	partnersButton(QIcon(Path::icon("partner.png")),
			TidPartnersButton, TidContext, QKeySequence(Qt::Key_F5)),
	companyButton(QIcon(Path::icon("company.png")),
			TidCompanyButton, TidContext, QKeySequence(Qt::Key_F5)),
	wareButton(QIcon(Path::icon("ware.png")),
			TidWareButton, TidContext, QKeySequence(Qt::Key_F6)),
	tagButton(QIcon(Path::icon("tag.png")),
			TidTagButton, TidContext, QKeySequence(Qt::Key_F7)),
	infoButton(QIcon(Path::icon("info.png")),
			TidInfoButton, TidContext, QKeySequence(Qt::Key_F8)),
	quitButton(QIcon(Path::icon("delete.png")),
			TidQuitButton, TidContext, QKeySequence(Qt::Key_F9)),
	shoppingView(NULL),
	newItemView(NULL),
	customView(NULL),
	partnersView(NULL),
	companyView(NULL),
	waresView(NULL),
	tagsView(NULL),
	queryOptionsView(NULL),
	infoView(NULL)
{
	setWindowIcon(QIcon(Path::icon("butler.png")));
	setAttribute(Qt::WA_QuitOnClose, true);

	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(accept()));

//	connect(&todoButton, SIGNAL(clicked()), this, SLOT(openTodoView()));
	connect(&shoppingButton, SIGNAL(clicked()), this, SLOT(openShoppingView()));
	connect(&newItemButton, SIGNAL(clicked()), this, SLOT(openEditItemView()));
	connect(&analiticsButton, SIGNAL(clicked()), this, SLOT(openCustomView()));
	connect(&partnersButton, SIGNAL(clicked()), this, SLOT(openPartnersView()));
	connect(&companyButton, SIGNAL(clicked()), this, SLOT(openCompanyView()));
	connect(&wareButton, SIGNAL(clicked()), this, SLOT(openWaresView()));
	connect(&tagButton, SIGNAL(clicked()), this, SLOT(openTagsView()));
	connect(&infoButton, SIGNAL(clicked()), this, SLOT(openInfoView()));
	connect(&quitButton, SIGNAL(clicked()), this, SLOT(accept()));

	retranslate();
	loadState();
}

MainView::~MainView()
{
//	delete todoView;
	delete shoppingView;
	delete newItemView;
	delete customView;
	delete partnersView;
	delete companyView;
	delete tagsView;
	delete waresView;
	delete queryOptionsView;
	delete infoView;
}

void MainView::retranslate()
{
	setWindowTitle(tr(TidMainWindowTitle).arg(PKGNAME));

	relayout();
}

void MainView::applyLayout()
{
	HLayout * mainLayout = new HLayout;

	VLayout * layout = new VLayout;

	layout->addStretch(0);

//	layout->addWidget(&todoButton);
//	layout->addWidget(&shoppingButton);
	layout->addWidget(&newItemButton);
	layout->addWidget(&analiticsButton);
	layout->addWidget(&wareButton);
	layout->addWidget(&tagButton);
	layout->addWidget(&partnersButton);
	layout->addWidget(&companyButton);
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
	todoButton.expanding();
	shoppingButton.expanding();
	newItemButton.expanding();
	analiticsButton.expanding();
	partnersButton.expanding();
	companyButton.expanding();
	wareButton.expanding();
	tagButton.expanding();
	infoButton.expanding();
	quitButton.expanding();

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
	relayout();
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

//	if(settings.value(prefix + "/todoview", false).toBool())
//		openTodoView();
	if(settings.value(prefix + "/shoppingview", false).toBool())
		openShoppingView();
	if(settings.value(prefix + "/newitemview", false).toBool())
		openEditItemView();
	if(settings.value(prefix + "/customview", false).toBool())
		openCustomView();
	if(settings.value(prefix + "/partnersview", false).toBool())
		openPartnersView();
	if(settings.value(prefix + "/companyview", false).toBool())
		openCompanyView();
	if(settings.value(prefix + "/waresview", false).toBool())
		openWaresView();
	if(settings.value(prefix + "/tagsview", false).toBool())
		openTagsView();
	if(settings.value(prefix + "/queryoptionsview", false).toBool())
		openQueryOptionsView();
	if(settings.value(prefix + "/infoview", false).toBool())
		openInfoView();

	QTimer::singleShot(0, this, SLOT(activateSavedActiveWindow()));
}

void MainView::activateSavedActiveWindow()
{
	QString prefix("MainView");
	QSettings settings;
	PannView * activeWindow = 0;
	QString activeWindowName = settings.value(
			prefix + "/activeWindow", false).toString();
	if(activeWindowName == "mainView")
		activeWindow = this;
	else if(activeWindowName == "shoppingView")
		activeWindow = shoppingView;
	else if(activeWindowName == "newItemView")
		activeWindow = newItemView;
	else if(activeWindowName == "customView")
		activeWindow = customView;
	else if(activeWindowName == "partnersView")
		activeWindow = partnersView;
	else if(activeWindowName == "companyView")
		activeWindow = companyView;
	else if(activeWindowName == "waresView")
		activeWindow = waresView;
	else if(activeWindowName == "tagsView")
		activeWindow = tagsView;
	else if(activeWindowName == "queryOptionsView")
		activeWindow = queryOptionsView;
	else if(activeWindowName == "infoView")
		activeWindow = infoView;
	if(activeWindow)
		activeWindow->activate();
}

void MainView::saveState()
{
	QString prefix("MainView");
	PannView::saveState(prefix);
	QSettings settings;

//	SAVE_VIEW_STATE(todoView);
	SAVE_VIEW_STATE(shoppingView);
	SAVE_VIEW_STATE(newItemView);
	SAVE_VIEW_STATE(customView);
	SAVE_VIEW_STATE(partnersView);
	SAVE_VIEW_STATE(companyView);
	SAVE_VIEW_STATE(tagsView);
	SAVE_VIEW_STATE(waresView);
	SAVE_VIEW_STATE(tagsView);
	SAVE_VIEW_STATE(queryOptionsView);
	SAVE_VIEW_STATE(infoView);

	QWidget * activeWindow = QApplication::activeWindow();
	const char * activeWindowName = 0;
	if(activeWindow == this)
		activeWindowName = "mainView";
	else if(activeWindow == shoppingView)
		activeWindowName = "shoppingView";
	else if(activeWindow == newItemView)
		activeWindowName = "newItemView";
	else if(activeWindow == customView)
		activeWindowName = "customView";
	else if(activeWindow == partnersView)
		activeWindowName = "partnersView";
	else if(activeWindow == companyView)
		activeWindowName = "companyView";
	else if(activeWindow == waresView)
		activeWindowName = "waresView";
	else if(activeWindow == tagsView)
		activeWindowName = "tagsView";
	else if(activeWindow == queryOptionsView)
		activeWindowName = "queryOptionsView";
	else if(activeWindow == infoView)
		activeWindowName = "infoView";
	settings.setValue(prefix + "/activeWindow", activeWindowName);

	settings.setValue(prefix + "/dbfile", databaseDescriptor(dbname).databaseName);
}

void MainView::openTodoView()
{
/*	if(!todoView)
		todoView = new TodoView(dbname);
	todoView->activate();*/
}

void MainView::openShoppingView()
{
	if(!shoppingView)
		shoppingView = new ShoppingView(dbname);
	shoppingView->activate();
}

void MainView::openEditItemView()
{
	if(!newItemView)
		newItemView = new EditItemView(dbname, *model);
	newItemView->activate();
}

void MainView::openCustomView()
{
	if(!customView)
		customView = new CustomView(dbname);

	if(!customView->isVisible()){
		customView->activate();
	} else {
		CustomView *anotherCustomView;
		anotherCustomView = new CustomView(dbname);
		anotherCustomView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherCustomView->activate();
	}
}

void MainView::openPartnersView()
{
	if(!partnersView)
		partnersView = new PartnersView(dbname);
	partnersView->activate();
}

void MainView::openCompanyView()
{
	if(!companyView)
		companyView = new CompanyView(dbname);
	companyView->activate();
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
