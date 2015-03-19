/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2014 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_accountsview.h"
#include "butler_editaccountview.h"
#include "butler_config.h"

SCC TidContext = "AccountsView";

SCC TidAccountsWindowTitle = QT_TRANSLATE_NOOP("AccountsView", "Business accounts");
SCC TidAddButton = QT_TRANSLATE_NOOP("AccountsView", "Add new account");
SCC TidEditButton = QT_TRANSLATE_NOOP("AccountsView", "Edit account");
SCC TidDelButton = QT_TRANSLATE_NOOP("AccountsView", "Delete account");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("AccountsView", "Refresh account list");

AccountsView::AccountsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(accountModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newAccountView(NULL),
	editAccountView(NULL)
{
	setWindowIcon(QIcon(Path::icon("account.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Account::LastModified);

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
		titlePrefix = dbname.str;
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
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Account::SwiftCode, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Account::SwiftCode, QHeaderView::ResizeToContents);

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
		QTimer::singleShot(0, this, SLOT(editItem()));

	updateToolButtonStates();
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
