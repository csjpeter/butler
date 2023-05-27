/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_config.h"
#include "butler_listviews.h"

SCC TidCompanyContext = "CompanyView";

SCC TidCompanysWindowTitle = QT_TRANSLATE_NOOP("CompanyView", "Companies");
SCC TidCompanyAddButton = QT_TRANSLATE_NOOP("CompanyView", "Add new company");
SCC TidCompanyEditButton = QT_TRANSLATE_NOOP("CompanyView", "Edit company");
SCC TidCompanyDelButton = QT_TRANSLATE_NOOP("CompanyView", "Delete company");
SCC TidCompanyRefreshButton = QT_TRANSLATE_NOOP("CompanyView", "Refresh company list");

SCC TidPartnerContext = "PartnersView";

SCC TidPartnersWindowTitle = QT_TRANSLATE_NOOP("PartnersView", "Business partners");
SCC TidPartnerAddButton = QT_TRANSLATE_NOOP("PartnersView", "Add new partner");
SCC TidPartnerEditButton = QT_TRANSLATE_NOOP("PartnersView", "Edit partner");
SCC TidPartnerDelButton = QT_TRANSLATE_NOOP("PartnersView", "Delete partner");
SCC TidPartnerRefreshButton = QT_TRANSLATE_NOOP("PartnersView", "Refresh partner list");

SCC TidAccountContext = "AccountsView";

SCC TidAccountsWindowTitle = QT_TRANSLATE_NOOP("AccountsView", "Business accounts");
SCC TidAccountAddButton = QT_TRANSLATE_NOOP("AccountsView", "Add new account");
SCC TidAccountEditButton = QT_TRANSLATE_NOOP("AccountsView", "Edit account");
SCC TidAccountDelButton = QT_TRANSLATE_NOOP("AccountsView", "Delete account");
SCC TidAccountRefreshButton = QT_TRANSLATE_NOOP("AccountsView", "Refresh account list");

SCC TidWareContext = "WaresView";

SCC TidWaresWindowTitle = QT_TRANSLATE_NOOP("WaresView", "Ware/Service list");
SCC TidWareAddButton = QT_TRANSLATE_NOOP("WaresView", "Add new ware or service");
SCC TidWareEditButton = QT_TRANSLATE_NOOP("WaresView", "Edit ware or service");
SCC TidWareDelButton = QT_TRANSLATE_NOOP("WaresView", "Delete ware or service");
SCC TidWareRefreshButton = QT_TRANSLATE_NOOP("WaresView", "Refresh ware/service list");

SCC TidTagContext = "TagsView";

SCC TidTagsWindowTitle = QT_TRANSLATE_NOOP("TagsView", "Tag list");
SCC TidTagAddButton = QT_TRANSLATE_NOOP("TagsView", "Add new tag");
SCC TidTagEditButton = QT_TRANSLATE_NOOP("TagsView", "Edit tag");
SCC TidTagDelButton = QT_TRANSLATE_NOOP("TagsView", "Delete tag");
SCC TidTagRefreshButton = QT_TRANSLATE_NOOP("TagsView", "Refresh tag list");

SCC TidBrandContext = "BrandsView";

SCC TidBrandsWindowTitle = QT_TRANSLATE_NOOP("BrandsView", "Brand list");
SCC TidBrandAddButton = QT_TRANSLATE_NOOP("BrandsView", "Add new brand");
SCC TidBrandEditButton = QT_TRANSLATE_NOOP("BrandsView", "Edit brand");
SCC TidBrandDelButton = QT_TRANSLATE_NOOP("BrandsView", "Delete brand");
SCC TidBrandRefreshButton = QT_TRANSLATE_NOOP("BrandsView", "Refresh brand list");

SCC TidInventoryContext = "InventoriesView";

SCC TidInventorysWindowTitle = QT_TRANSLATE_NOOP("InventoriesView", "Inventory list");
SCC TidInventoryAddButton = QT_TRANSLATE_NOOP("InventoriesView", "Add new inventory");
SCC TidInventoryEditButton = QT_TRANSLATE_NOOP("InventoriesView", "Edit inventory");
SCC TidInventoryDelButton = QT_TRANSLATE_NOOP("InventoriesView", "Delete inventory");
SCC TidInventoryRefreshButton = QT_TRANSLATE_NOOP("InventoriesView", "Refresh inventory list");































#include "butler_editcompanyview.h"

CompanysView::CompanysView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(companyModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidCompanyAddButton, TidCompanyContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidCompanyDelButton, TidCompanyContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidCompanyEditButton, TidCompanyContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidCompanyRefreshButton, TidCompanyContext, QKeySequence(QKeySequence::Refresh)),
	newCompanyView(NULL),
	editCompanyView(NULL)
{
	setWindowIcon(QIcon(Path::icon("company.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Company::LastModified);
	tableView.hideColumn(Company::Deleted);
	tableView.hideColumn(Company::NumOfFields);
	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newCompany()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editCompany()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delCompany()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

CompanysView::~CompanysView()
{
	delete newCompanyView;
	delete editCompanyView;
}

void CompanysView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidCompanysWindowTitle));
	relayout();
}

void CompanysView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void CompanysView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Company::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Company::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void CompanysView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void CompanysView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void CompanysView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void CompanysView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void CompanysView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void CompanysView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void CompanysView::loadState()
{
	QString prefix("CompanysView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editCompanyView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editCompany()));
}

void CompanysView::saveState()
{
	QString prefix("CompanysView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editCompanyView);
}

void CompanysView::newCompany()
{
	if(!newCompanyView)
		newCompanyView = new EditCompanyView(dbname);

	newCompanyView->activate();
}

void CompanysView::editCompany()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select company first."));
		return;
	}

	if(!editCompanyView)
		editCompanyView = new EditCompanyView(dbname);

	editCompanyView->setCursor(tableView.currentIndex());
	editCompanyView->activate();
}

void CompanysView::delCompany()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select company first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Company & company = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a company"),
			tr("Shall we delete this company: ") + company.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void CompanysView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void CompanysView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void CompanysView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




#include "butler_editpartnerview.h"

PartnersView::PartnersView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(partnerModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidPartnerAddButton, TidPartnerContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidPartnerDelButton, TidPartnerContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidPartnerEditButton, TidPartnerContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidPartnerRefreshButton, TidPartnerContext, QKeySequence(QKeySequence::Refresh)),
	newPartnerView(NULL),
	editPartnerView(NULL)
{
	setWindowIcon(QIcon(Path::icon("partner.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Partner::LastModified);
	tableView.hideColumn(Partner::Deleted);
	tableView.hideColumn(Partner::NumOfFields);
	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newPartner()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editPartner()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delPartner()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

PartnersView::~PartnersView()
{
	delete newPartnerView;
	delete editPartnerView;
}

void PartnersView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidPartnersWindowTitle));
	relayout();
}

void PartnersView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void PartnersView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Partner::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Partner::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void PartnersView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void PartnersView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void PartnersView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void PartnersView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void PartnersView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void PartnersView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void PartnersView::loadState()
{
	QString prefix("PartnersView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editPartnerView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editPartner()));
}

void PartnersView::saveState()
{
	QString prefix("PartnersView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editPartnerView);
}

void PartnersView::newPartner()
{
	if(!newPartnerView)
		newPartnerView = new EditPartnerView(dbname);

	newPartnerView->activate();
}

void PartnersView::editPartner()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select partner first."));
		return;
	}

	if(!editPartnerView)
		editPartnerView = new EditPartnerView(dbname);

	editPartnerView->setCursor(tableView.currentIndex());
	editPartnerView->activate();
}

void PartnersView::delPartner()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select partner first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Partner & partner = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a partner"),
			tr("Shall we delete this partner: ") + partner.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void PartnersView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void PartnersView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void PartnersView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




#include "butler_editaccountview.h"

AccountsView::AccountsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(accountModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAccountAddButton, TidAccountContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidAccountDelButton, TidAccountContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidAccountEditButton, TidAccountContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidAccountRefreshButton, TidAccountContext, QKeySequence(QKeySequence::Refresh)),
	newAccountView(NULL),
	editAccountView(NULL)
{
	setWindowIcon(QIcon(Path::icon("account.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Account::LastModified);
	tableView.hideColumn(Account::Deleted);
	tableView.hideColumn(Account::NumOfFields);
	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newAccount()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editAccount()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delAccount()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

AccountsView::~AccountsView()
{
	delete newAccountView;
	delete editAccountView;
}

void AccountsView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidAccountsWindowTitle));
	relayout();
}

void AccountsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void AccountsView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Account::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Account::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void AccountsView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void AccountsView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void AccountsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void AccountsView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void AccountsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void AccountsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void AccountsView::loadState()
{
	QString prefix("AccountsView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editAccountView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editAccount()));
}

void AccountsView::saveState()
{
	QString prefix("AccountsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editAccountView);
}

void AccountsView::newAccount()
{
	if(!newAccountView)
		newAccountView = new EditAccountView(dbname);

	newAccountView->activate();
}

void AccountsView::editAccount()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select account first."));
		return;
	}

	if(!editAccountView)
		editAccountView = new EditAccountView(dbname);

	editAccountView->setCursor(tableView.currentIndex());
	editAccountView->activate();
}

void AccountsView::delAccount()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select account first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Account & account = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a account"),
			tr("Shall we delete this account: ") + account.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void AccountsView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void AccountsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void AccountsView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




#include "butler_editwareview.h"

WaresView::WaresView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(wareModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidWareAddButton, TidWareContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidWareDelButton, TidWareContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidWareEditButton, TidWareContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidWareRefreshButton, TidWareContext, QKeySequence(QKeySequence::Refresh)),
	newWareView(NULL),
	editWareView(NULL)
{
	setWindowIcon(QIcon(Path::icon("ware.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Ware::LastModified);
	tableView.hideColumn(Ware::Deleted);
	tableView.hideColumn(Ware::NumOfFields);
tableView.hideColumn(Ware::Types);
	tableView.hideColumn(Ware::Tags);
		toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newWare()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editWare()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delWare()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

WaresView::~WaresView()
{
	delete newWareView;
	delete editWareView;
}

void WaresView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidWaresWindowTitle));
	relayout();
}

void WaresView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void WaresView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Ware::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Ware::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void WaresView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void WaresView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void WaresView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void WaresView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void WaresView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void WaresView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void WaresView::loadState()
{
	QString prefix("WaresView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editWareView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editWare()));
}

void WaresView::saveState()
{
	QString prefix("WaresView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editWareView);
}

void WaresView::newWare()
{
	if(!newWareView)
		newWareView = new EditWareView(dbname);

	newWareView->activate();
}

void WaresView::editWare()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select ware first."));
		return;
	}

	if(!editWareView)
		editWareView = new EditWareView(dbname);

	editWareView->setCursor(tableView.currentIndex());
	editWareView->activate();
}

void WaresView::delWare()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select ware first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Ware & ware = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a ware"),
			tr("Shall we delete this ware: ") + ware.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void WaresView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void WaresView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void WaresView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




#include "butler_edittagview.h"

TagsView::TagsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(tagModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidTagAddButton, TidTagContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidTagDelButton, TidTagContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidTagEditButton, TidTagContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidTagRefreshButton, TidTagContext, QKeySequence(QKeySequence::Refresh)),
	newTagView(NULL),
	editTagView(NULL)
{
	setWindowIcon(QIcon(Path::icon("tag.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Tag::LastModified);
	tableView.hideColumn(Tag::Deleted);
	tableView.hideColumn(Tag::NumOfFields);
	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newTag()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editTag()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delTag()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

TagsView::~TagsView()
{
	delete newTagView;
	delete editTagView;
}

void TagsView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidTagsWindowTitle));
	relayout();
}

void TagsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void TagsView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Tag::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Tag::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void TagsView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void TagsView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void TagsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void TagsView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void TagsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void TagsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void TagsView::loadState()
{
	QString prefix("TagsView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editTagView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editTag()));
}

void TagsView::saveState()
{
	QString prefix("TagsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editTagView);
}

void TagsView::newTag()
{
	if(!newTagView)
		newTagView = new EditTagView(dbname);

	newTagView->activate();
}

void TagsView::editTag()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select tag first."));
		return;
	}

	if(!editTagView)
		editTagView = new EditTagView(dbname);

	editTagView->setCursor(tableView.currentIndex());
	editTagView->activate();
}

void TagsView::delTag()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select tag first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Tag & tag = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a tag"),
			tr("Shall we delete this tag: ") + tag.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void TagsView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void TagsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void TagsView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




#include "butler_editbrandview.h"

BrandsView::BrandsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(brandModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidBrandAddButton, TidBrandContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidBrandDelButton, TidBrandContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidBrandEditButton, TidBrandContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidBrandRefreshButton, TidBrandContext, QKeySequence(QKeySequence::Refresh)),
	newBrandView(NULL),
	editBrandView(NULL)
{
	setWindowIcon(QIcon(Path::icon("brand.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Brand::LastModified);
	tableView.hideColumn(Brand::Deleted);
	tableView.hideColumn(Brand::NumOfFields);
	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newBrand()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editBrand()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delBrand()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

BrandsView::~BrandsView()
{
	delete newBrandView;
	delete editBrandView;
}

void BrandsView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidBrandsWindowTitle));
	relayout();
}

void BrandsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void BrandsView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Brand::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Brand::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void BrandsView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void BrandsView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void BrandsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void BrandsView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void BrandsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void BrandsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void BrandsView::loadState()
{
	QString prefix("BrandsView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editBrandView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editBrand()));
}

void BrandsView::saveState()
{
	QString prefix("BrandsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editBrandView);
}

void BrandsView::newBrand()
{
	if(!newBrandView)
		newBrandView = new EditBrandView(dbname);

	newBrandView->activate();
}

void BrandsView::editBrand()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select brand first."));
		return;
	}

	if(!editBrandView)
		editBrandView = new EditBrandView(dbname);

	editBrandView->setCursor(tableView.currentIndex());
	editBrandView->activate();
}

void BrandsView::delBrand()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select brand first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Brand & brand = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a brand"),
			tr("Shall we delete this brand: ") + brand.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void BrandsView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void BrandsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void BrandsView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




#include "butler_editinventoryview.h"

InventorysView::InventorysView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(inventoryModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidInventoryAddButton, TidInventoryContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidInventoryDelButton, TidInventoryContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidInventoryEditButton, TidInventoryContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidInventoryRefreshButton, TidInventoryContext, QKeySequence(QKeySequence::Refresh)),
	newInventoryView(NULL),
	editInventoryView(NULL)
{
	setWindowIcon(QIcon(Path::icon("inventory.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Inventory::LastModified);
	tableView.hideColumn(Inventory::Deleted);
	tableView.hideColumn(Inventory::NumOfFields);
	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newInventory()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editInventory()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delInventory()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

InventorysView::~InventorysView()
{
	delete newInventoryView;
	delete editInventoryView;
}

void InventorysView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidInventorysWindowTitle));
	relayout();
}

void InventorysView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void InventorysView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Inventory::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Inventory::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void InventorysView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void InventorysView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void InventorysView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void InventorysView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void InventorysView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void InventorysView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void InventorysView::loadState()
{
	QString prefix("InventorysView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editInventoryView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editInventory()));
}

void InventorysView::saveState()
{
	QString prefix("InventorysView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editInventoryView);
}

void InventorysView::newInventory()
{
	if(!newInventoryView)
		newInventoryView = new EditInventoryView(dbname);

	newInventoryView->activate();
}

void InventorysView::editInventory()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select inventory first."));
		return;
	}

	if(!editInventoryView)
		editInventoryView = new EditInventoryView(dbname);

	editInventoryView->setCursor(tableView.currentIndex());
	editInventoryView->activate();
}

void InventorysView::delInventory()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select inventory first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Inventory & inventory = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a inventory"),
			tr("Shall we delete this inventory: ") + inventory.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void InventorysView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void InventorysView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void InventorysView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}




