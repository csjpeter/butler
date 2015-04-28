/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_databases.h"

#include "butler_databasesview.h"
#include "butler_paymentqueryoptionsview.h"
#include "butler_queryoptionsview.h"
#include "butler_infoview.h"
#include "butler_edititemview.h"
#include "butler_paymentview.h"
#include "butler_customview.h"

#include "butler_config.h"

#include "butler_mainview.h"

SCC TidContext = "MainView";

SCC TidMainWindowTitle = QT_TRANSLATE_NOOP("MainView", "%1 - main view");
SCC TidTodoButton = QT_TRANSLATE_NOOP("MainView", "Todo notes");
SCC TidShoppingButton = QT_TRANSLATE_NOOP("MainView", "Shopping list");
SCC TidNewItemButton = QT_TRANSLATE_NOOP("MainView", "Expense/Income");
SCC TidAnaliticsButton = QT_TRANSLATE_NOOP("MainView", "Analitics / History");
SCC TidPaymentButton = QT_TRANSLATE_NOOP("MainView", "Payment / History");
SCC TidCompanyButton = QT_TRANSLATE_NOOP("MainView", "Companies");
SCC TidPartnersButton = QT_TRANSLATE_NOOP("MainView", "Partners");
SCC TidAccountsButton = QT_TRANSLATE_NOOP("MainView", "Accounts");
SCC TidWareButton = QT_TRANSLATE_NOOP("MainView", "Wares");
SCC TidTagButton = QT_TRANSLATE_NOOP("MainView", "Tags");
SCC TidBrandButton = QT_TRANSLATE_NOOP("MainView", "Brands");
SCC TidInventoryButton = QT_TRANSLATE_NOOP("MainView", "Inventories");
SCC TidDatabasesButton = QT_TRANSLATE_NOOP("MainView", "Databases");
SCC TidInfoButton = QT_TRANSLATE_NOOP("MainView", "Legal informations");
SCC TidQuitButton = QT_TRANSLATE_NOOP("MainView", "Quit");
SCC TidCurrentDbInfo = QT_TRANSLATE_NOOP("MainView", "Current database: %1");

/* Search for images using google:
https://www.googleapis.com/customsearch/v1?key=AIzaSyBz1WwkrfA8UursvLiUqRTJ2nC5bytMHQk&fileType=jpg&fileType=ong&cr=hu&searchType=image&cx=007625690417709007201:tyovxvdl-ek&q=tolnatej
*/

MainView::MainView(QWidget *parent) :
	PannView(parent),
	todoButton(QIcon(Path::icon("list.png")),
			TidTodoButton, TidContext, QKeySequence(Qt::Key_F1)),
	shoppingButton(QIcon(Path::icon("shopping.png")),
			TidShoppingButton, TidContext, QKeySequence(Qt::Key_F2)),
	newItemButton(QIcon(Path::icon("accounting.png")),
			TidNewItemButton, TidContext, QKeySequence(Qt::Key_F3)),
	analiticsButton(QIcon(Path::icon("analitics.png")),
			TidAnaliticsButton, TidContext, QKeySequence(Qt::Key_F5)),
	paymentButton(QIcon(Path::icon("analitics.png")),
			TidPaymentButton, TidContext, QKeySequence(Qt::Key_F4)),
	companyButton(QIcon(Path::icon("company.png")),
			TidCompanyButton, TidContext, QKeySequence(Qt::Key_F6)),
	partnersButton(QIcon(Path::icon("partner.png")),
			TidPartnersButton, TidContext, QKeySequence(Qt::Key_F7)),
	accountsButton(QIcon(Path::icon("account.png")),
			TidAccountsButton, TidContext, QKeySequence(Qt::Key_F8)),
	wareButton(QIcon(Path::icon("ware.png")),
			TidWareButton, TidContext, QKeySequence(Qt::Key_F9)),
	tagButton(QIcon(Path::icon("tag.png")),
			TidTagButton, TidContext, QKeySequence(Qt::Key_F10)),
	brandButton(QIcon(Path::icon("brand.png")),
			TidBrandButton, TidContext, QKeySequence(Qt::Key_F11)),
	inventoryButton(QIcon(Path::icon("inventory.png")),
			TidInventoryButton, TidContext, QKeySequence(Qt::Key_F12)),
	databasesButton(QIcon(Path::icon("databases.png")),
			TidDatabasesButton, TidContext, QKeySequence(Qt::Key_F13)),
	infoButton(QIcon(Path::icon("info.png")),
			TidInfoButton, TidContext, QKeySequence(Qt::Key_F14)),
	quitButton(QIcon(Path::icon("delete.png")),
			TidQuitButton, TidContext, QKeySequence(Qt::Key_F15)),
	//shoppingView(NULL),
	newItemView(NULL),
	customView(NULL),
	paymentView(NULL),
	companyView(NULL),
	partnersView(NULL),
	accountsView(NULL),
	waresView(NULL),
	tagsView(NULL),
	brandsView(NULL),
	inventoriesView(NULL),
	databasesView(NULL),
	paymentQueryOptionsView(NULL),
	itemQueryOptionsView(NULL),
	infoView(NULL)
{
	setWindowIcon(QIcon(Path::icon("butler.png")));
	setAttribute(Qt::WA_QuitOnClose, true);

	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(accept()));

//	connect(&todoButton, SIGNAL(clicked()), this, SLOT(openTodoView()));
//	connect(&shoppingButton, SIGNAL(clicked()), this, SLOT(openShoppingView()));
	connect(&newItemButton, SIGNAL(clicked()), this, SLOT(openEditItemView()));
	connect(&analiticsButton, SIGNAL(clicked()), this, SLOT(openCustomView()));
	connect(&paymentButton, SIGNAL(clicked()), this, SLOT(openPaymentView()));
	connect(&companyButton, SIGNAL(clicked()), this, SLOT(openCompanyView()));
	connect(&partnersButton, SIGNAL(clicked()), this, SLOT(openPartnersView()));
	connect(&accountsButton, SIGNAL(clicked()), this, SLOT(openAccountsView()));
	connect(&wareButton, SIGNAL(clicked()), this, SLOT(openWaresView()));
	connect(&tagButton, SIGNAL(clicked()), this, SLOT(openTagsView()));
	connect(&brandButton, SIGNAL(clicked()), this, SLOT(openBrandsView()));
	connect(&inventoryButton, SIGNAL(clicked()), this, SLOT(openInventoriesView()));
	connect(&databasesButton, SIGNAL(clicked()), this, SLOT(openDatabasesView()));
	connect(&infoButton, SIGNAL(clicked()), this, SLOT(openInfoView()));
	connect(&quitButton, SIGNAL(clicked()), this, SLOT(accept()));

	retranslate();
	loadState();
}

MainView::~MainView()
{
//	delete todoView;
	//delete shoppingView;
	delete newItemView;
	delete customView;
	delete paymentView;
	delete accountsView;
	delete partnersView;
	delete companyView;
	delete tagsView;
	delete inventoriesView;
	delete brandsView;
	delete databasesView;
	delete waresView;
	delete paymentQueryOptionsView;
	delete itemQueryOptionsView;
	delete infoView;
}

void MainView::retranslate()
{
	setWindowTitle(tr(TidMainWindowTitle).arg(PKGNAME));
	activeDbChanged();

	relayout();
}

void MainView::applyLayout()
{
	HLayout * mainLayout = new HLayout;

	VLayout * layout = new VLayout;

	layout->addWidget(&infoLabel);

	layout->addStretch(0);

//	layout->addWidget(&todoButton);
//	layout->addWidget(&shoppingButton);
	layout->addWidget(&newItemButton);
	layout->addWidget(&analiticsButton);
	layout->addWidget(&paymentButton);
	layout->addWidget(&wareButton);
	layout->addWidget(&tagButton);
	layout->addWidget(&brandButton);
	layout->addWidget(&inventoryButton);
	layout->addWidget(&partnersButton);
	layout->addWidget(&accountsButton);
	layout->addWidget(&companyButton);
	layout->addWidget(&databasesButton);
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
	paymentButton.expanding();
	companyButton.expanding();
	partnersButton.expanding();
	accountsButton.expanding();
	wareButton.expanding();
	tagButton.expanding();
	brandButton.expanding();
	inventoryButton.expanding();
	inventoryButton.expanding();
	databasesButton.expanding();
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

	try {
		//if(settings.value(prefix + "/todoview", false).toBool())
		//	openTodoView();
		//if(settings.value(prefix + "/shoppingview", false).toBool())
		//	openShoppingView();
		if(settings.value(prefix + "/newitemview", false).toBool())
			openEditItemView();
		if(settings.value(prefix + "/paymentview", false).toBool())
			openPaymentView();
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
		if(settings.value(prefix + "/tagsview", false).toBool())
			openBrandsView();
		if(settings.value(prefix + "/databasesview", false).toBool())
			openDatabasesView();
		if(settings.value(prefix + "/paymentqueryoptionsview", false).toBool())
			openPaymentQueryOptionsView();
		if(settings.value(prefix + "/itemqueryoptionsview", false).toBool())
			openItemQueryOptionsView();
		if(settings.value(prefix + "/infoview", false).toBool())
			openInfoView();

		QTimer::singleShot(0, this, SLOT(activateSavedActiveWindow()));
	} catch (DbError & e) {
		EXCEPTION(e);
	}
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
	//else if(activeWindowName == "shoppingView")
	//	activeWindow = shoppingView;
	else if(activeWindowName == "newItemView")
		activeWindow = newItemView;
	else if(activeWindowName == "customView")
		activeWindow = customView;
	else if(activeWindowName == "paymentView")
		activeWindow = paymentView;
	else if(activeWindowName == "partnersView")
		activeWindow = partnersView;
	else if(activeWindowName == "companyView")
		activeWindow = companyView;
	else if(activeWindowName == "waresView")
		activeWindow = waresView;
	else if(activeWindowName == "tagsView")
		activeWindow = tagsView;
	else if(activeWindowName == "brandsView")
		activeWindow = brandsView;
	else if(activeWindowName == "inventoriesView")
		activeWindow = inventoriesView;
	else if(activeWindowName == "databasesView")
		activeWindow = databasesView;
	else if(activeWindowName == "paymentQueryOptionsView")
		activeWindow = paymentQueryOptionsView;
	else if(activeWindowName == "itemQueryOptionsView")
		activeWindow = itemQueryOptionsView;
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
	//SAVE_VIEW_STATE(shoppingView);
	SAVE_VIEW_STATE(newItemView);
	SAVE_VIEW_STATE(customView);
	SAVE_VIEW_STATE(paymentView);
	SAVE_VIEW_STATE(partnersView);
	SAVE_VIEW_STATE(companyView);
	SAVE_VIEW_STATE(tagsView);
	SAVE_VIEW_STATE(brandsView);
	SAVE_VIEW_STATE(inventoriesView);
	SAVE_VIEW_STATE(waresView);
	SAVE_VIEW_STATE(databasesView);
	SAVE_VIEW_STATE(paymentQueryOptionsView);
	SAVE_VIEW_STATE(itemQueryOptionsView);
	SAVE_VIEW_STATE(infoView);

	QWidget * activeWindow = QApplication::activeWindow();
	const char * activeWindowName = 0;
	if(activeWindow == this)
		activeWindowName = "mainView";
	//else if(activeWindow == shoppingView)
	//	activeWindowName = "shoppingView";
	else if(activeWindow == newItemView)
		activeWindowName = "newItemView";
	else if(activeWindow == customView)
		activeWindowName = "customView";
	else if(activeWindow == paymentView)
		activeWindowName = "paymentView";
	else if(activeWindow == partnersView)
		activeWindowName = "partnersView";
	else if(activeWindow == companyView)
		activeWindowName = "companyView";
	else if(activeWindow == waresView)
		activeWindowName = "waresView";
	else if(activeWindow == tagsView)
		activeWindowName = "tagsView";
	else if(activeWindow == brandsView)
		activeWindowName = "brandsView";
	else if(activeWindow == inventoriesView)
		activeWindowName = "inventoriesView";
	else if(activeWindow == databasesView)
		activeWindowName = "databasesView";
	else if(activeWindow == paymentQueryOptionsView)
		activeWindowName = "paymentQueryOptionsView";
	else if(activeWindow == itemQueryOptionsView)
		activeWindowName = "itemQueryOptionsView";
	else if(activeWindow == infoView)
		activeWindowName = "infoView";
	settings.setValue(prefix + "/activeWindow", activeWindowName);
}

void MainView::activeDbChanged()
{
	if(Config::defaultDbName != "localdb")
		infoLabel.setText(tr(TidCurrentDbInfo).arg(Config::defaultDbName.str));
	else
		infoLabel.setText("");
}
/*
void MainView::openTodoView()
{
	if(!todoView)
		todoView = new TodoView(Config::defaultDbName);
	todoView->activate();
}*/
/*
void MainView::openShoppingView()
{
	if(!shoppingView)
		shoppingView = new ShoppingView(Config::defaultDbName);
	if(shoppingView->dbname == Config::defaultDbName){
		shoppingView->activate();
	} else {
		ShoppingView *anotherShoppingView;
		anotherShoppingView = new ShoppingView(Config::defaultDbName);
		anotherShoppingView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherShoppingView->activate();
	}
}
*/
void MainView::openEditItemView()
{
	if(!newItemView)
		newItemView = EditItemView::newItemViewFactory(Config::defaultDbName);
	if(newItemView->dbname == Config::defaultDbName){
		newItemView->activate();
	} else {
		EditItemView *anotherEditItemView;
		anotherEditItemView = EditItemView::newItemViewFactory(Config::defaultDbName);
		anotherEditItemView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherEditItemView->activate();
	}
}

void MainView::openCustomView()
{
	if(!customView)
		customView = new CustomView(Config::defaultDbName);

	if(!customView->isVisible() && customView->dbname == Config::defaultDbName){
		customView->activate();
	} else {
		CustomView *anotherCustomView;
		anotherCustomView = new CustomView(Config::defaultDbName);
		anotherCustomView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherCustomView->activate();
	}
}

void MainView::openPaymentView()
{
	if(!paymentView)
		paymentView = new PaymentView(Config::defaultDbName);

	if(!paymentView->isVisible() && paymentView->dbname == Config::defaultDbName){
		paymentView->activate();
	} else {
		PaymentView *anotherPaymentView;
		anotherPaymentView = new PaymentView(Config::defaultDbName);
		anotherPaymentView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherPaymentView->activate();
	}
}

void MainView::openCompanyView()
{
	if(!companyView)
		companyView = new CompanysView(Config::defaultDbName);
	if(companyView->dbname == Config::defaultDbName){
		companyView->activate();
	} else {
		CompanysView *anotherCompanyView;
		anotherCompanyView = new CompanysView(Config::defaultDbName);
		anotherCompanyView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherCompanyView->activate();
	}
}

void MainView::openPartnersView()
{
	if(!partnersView)
		partnersView = new PartnersView(Config::defaultDbName);
	if(partnersView->dbname == Config::defaultDbName){
		partnersView->activate();
	} else {
		PartnersView *anotherPartnersView;
		anotherPartnersView = new PartnersView(Config::defaultDbName);
		anotherPartnersView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherPartnersView->activate();
	}
}

void MainView::openAccountsView()
{
	if(!accountsView)
		accountsView = new AccountsView(Config::defaultDbName);
	if(accountsView->dbname == Config::defaultDbName){
		accountsView->activate();
	} else {
		AccountsView *anotherAccountsView;
		anotherAccountsView = new AccountsView(Config::defaultDbName);
		anotherAccountsView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherAccountsView->activate();
	}
}

void MainView::openWaresView()
{
	if(!waresView)
		waresView = new WaresView(Config::defaultDbName);
	if(waresView->dbname == Config::defaultDbName){
		waresView->activate();
	} else {
		WaresView *anotherWaresView;
		anotherWaresView = new WaresView(Config::defaultDbName);
		anotherWaresView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherWaresView->activate();
	}
}

void MainView::openDatabasesView()
{
	if(!databasesView){
		databasesView = new DatabasesView();
		connect(databasesView, SIGNAL(activeDbChanged()), this, SLOT(activeDbChanged()));
	}
	databasesView->activate();
}

void MainView::openTagsView()
{
	if(!tagsView)
		tagsView = new TagsView(Config::defaultDbName);
	if(tagsView->dbname == Config::defaultDbName){
		tagsView->activate();
	} else {
		TagsView *anotherTagsView;
		anotherTagsView = new TagsView(Config::defaultDbName);
		anotherTagsView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherTagsView->activate();
	}
}

void MainView::openBrandsView()
{
	if(!brandsView)
		brandsView = new BrandsView(Config::defaultDbName);
	if(brandsView->dbname == Config::defaultDbName){
		brandsView->activate();
	} else {
		BrandsView *anotherBrandsView;
		anotherBrandsView = new BrandsView(Config::defaultDbName);
		anotherBrandsView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherBrandsView->activate();
	}
}

void MainView::openInventoriesView()
{
	if(!inventoriesView)
		inventoriesView = new InventorysView(Config::defaultDbName);
	if(inventoriesView->dbname == Config::defaultDbName){
		inventoriesView->activate();
	} else {
		InventorysView *anotherInventoriesView;
		anotherInventoriesView = new InventorysView(Config::defaultDbName);
		anotherInventoriesView->setAttribute(Qt::WA_DeleteOnClose, true);
		anotherInventoriesView->activate();
	}
}

void MainView::openPaymentQueryOptionsView()
{
#if 0
	if(!paymentQueryOptionsView)
		paymentQueryOptionsView = new QueryOptionsView;
	paymentQueryOptionsView->activate();
#endif
}

void MainView::openItemQueryOptionsView()
{
#if 0
	if(!itemQueryOptionsView)
		itemQueryOptionsView = new QueryOptionsView;
	itemQueryOptionsView->activate();
#endif
}

void MainView::openInfoView()
{
	if(!infoView)
		infoView = new InfoView;
	infoView->activate();
}
