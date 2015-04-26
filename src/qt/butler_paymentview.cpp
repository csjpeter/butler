/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_paymentview.h"
#include "butler_paymentqueryoptionsview.h"
#include "butler_editpaymentview.h"
#include "butler_paymentstatsview.h"
#include "butler_config.h"

SCC TidContext = "PaymentView";

SCC TidAnaliticsWindowTitle = QT_TRANSLATE_NOOP("PaymentView", "Analitics");
SCC TidEditItemButton = QT_TRANSLATE_NOOP("PaymentView", "Edit item");
SCC TidDeleteItemButton = QT_TRANSLATE_NOOP("PaymentView", "Delete item");
SCC TidRefreshItemsButton = QT_TRANSLATE_NOOP("PaymentView", "Refresh items");
SCC TidShoppingItemButton = QT_TRANSLATE_NOOP("PaymentView", "Add item to shopping list");
SCC TidFilterItemButton = QT_TRANSLATE_NOOP("PaymentView", "Filter items");
SCC TidStatsItemButton = QT_TRANSLATE_NOOP("PaymentView", "Statistics");

PaymentView::PaymentView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(itemModel(dbname)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditItemButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDeleteItemButton, TidContext, QKeySequence(Qt::Key_F2)),
	shoppingButton(QIcon(Path::icon("shopping.png")),
			TidShoppingItemButton, TidContext, QKeySequence(Qt::Key_F3)),
	statsButton(QIcon(Path::icon("statistics.png")),
			TidStatsItemButton, TidContext, QKeySequence(Qt::Key_F4)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshItemsButton, TidContext, QKeySequence(QKeySequence::Refresh)),/*F5*/
	filterButton(QIcon(Path::icon("query.png")),
			TidFilterItemButton, TidContext, QKeySequence(Qt::Key_F6)),
	editItemView(NULL),
	queryOptsView(NULL),
	editWareView(NULL),
	statsView(NULL)
{
	setWindowIcon(QIcon(Path::icon("list.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Item::LastModified);

	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	//toolBar.addToolWidget(shoppingButton);
	toolBar.addToolWidget(statsButton);
	toolBar.addToolWidget(refreshButton);
	toolBar.addToolWidget(filterButton);

	connect(&editButton, SIGNAL(clicked()), this, SLOT(editItem()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delItem()));
	connect(&shoppingButton, SIGNAL(clicked()), this, SLOT(shoppingItem()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refreshItems()));
	connect(&filterButton, SIGNAL(clicked()), this, SLOT(filterItems()));
	connect(&statsButton, SIGNAL(clicked()), this, SLOT(statsItems()));
//	connect(&editWareButton, SIGNAL(clicked()), this, SLOT(editWare()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

PaymentView::~PaymentView()
{
	delete editItemView;
	delete queryOptsView;
	delete editWareView;
	delete statsView;
}

void PaymentView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix = dbname.str;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidAnaliticsWindowTitle));
	relayout();
}

void PaymentView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void PaymentView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Item::Comment, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Item::Comment, QHeaderView::ResizeToContents);

//	LayoutState newState = LayoutState::Wide;
//	newState = LayoutState::Expanding;
//	newState = LayoutState::Wide;
	applyLayout();

	if(width() < sizeHint().width()) {
//		newState = LayoutState::Medium;
		applyLayout();
	}

	if(width() < sizeHint().width()) {
//		newState = LayoutState::Narrow;
		applyLayout();
	}

	updateToolButtonStates();
}

void PaymentView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
		//shoppingButton.show();
	} else {
		editButton.hide();
		delButton.hide();
		shoppingButton.hide();
	}
	toolBar.updateButtons();
}

void PaymentView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void PaymentView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void PaymentView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void PaymentView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void PaymentView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void PaymentView::loadState()
{
	QString prefix("PaymentView");
	PannView::loadState(prefix);
	QSettings settings;

	Text queryName(settings.value(prefix + "/query", ""));
	QueryModel & qm = queryModel(dbname);
	if(qm.set.has(queryName))
		model->opts = qm.set.query(queryName);
	model->query();

	tableView.loadState(prefix);

	DateTime uploadDate(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model->set.has(uploadDate))
		tableView.setCurrentIndex(model->index(model->index(uploadDate), col));

	if(settings.value(prefix + "/editItemView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));

	if(settings.value(prefix + "/queryOptsView", false).toBool())
		QTimer::singleShot(0, this, SLOT(filterItems()));

	updateToolButtonStates();
}

void PaymentView::saveState()
{
	QString prefix("PaymentView");
	PannView::saveState(prefix);
	QSettings settings;

	settings.setValue(prefix + "/query", model->opts.name);

	QString uploadDate;
	if(tableView.currentIndex().isValid()){
		const Item & item = model->data(tableView.currentIndex().row());
		uploadDate = item.uploadDate.toString(Qt::ISODate);
	}
	settings.setValue(prefix + "/currentitem", uploadDate);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editItemView);
	SAVE_VIEW_STATE(queryOptsView);
}

void PaymentView::editItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first."));
		return;
	}

	if(!editItemView)
		editItemView = new EditItemView(dbname, *model);

	editItemView->setCursor(tableView.currentIndex());
	editItemView->activate();
}

void PaymentView::delItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Item & item = model->data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting an item"),
			tr("Shall we delete this item: ") + item.name +
			(item.type.size() ? (" (" + item.type + ")") : ""),
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model->del(row);
}

void PaymentView::refreshItems()
{
	DateTime uploadDate;
	if(tableView.currentIndex().isValid()){
		const Item & item = model->data(tableView.currentIndex().row());
		uploadDate = item.uploadDate;
	}

	model->query();

	if(model->set.has(uploadDate))
		tableView.setCurrentIndex(model->index(model->index(uploadDate), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void PaymentView::shoppingItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Item & item = model->data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Adding to shopping list"),
			QString("Shall we add a corresponding item to the shopping list:\n") +
			item.name + (item.type.size() ? (" (" + item.type + ")") : ""),
			QMessageBox::Yes | QMessageBox::No, 0, Qt::Dialog));
	int res = msg->exec();
	if(res == QMessageBox::Yes){
		//model->addShoppingItem(row);
	}
}

void PaymentView::editWare()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select an item first."));
		return;
	}

	WareModel & wm = wareModel(dbname);
	if(!editWareView)
		editWareView = new EditWareView(dbname);

	const Item & item = model->data(tableView.currentIndex().row());

	editWareView->setCursor(wm.index(wm.index(item.name), 0));
	editWareView->activate();
}

void PaymentView::filterItems()
{
	if(!queryOptsView){
		queryOptsView = new QueryOptionsView(dbname);
		connect(queryOptsView, SIGNAL(accepted()), this, SLOT(applyNewFilter()));
	}
	queryOptsView->query = model->opts;
	queryOptsView->activate();
}

void PaymentView::applyNewFilter()
{
	model->opts = queryOptsView->query;
	model->query();
	tableView.setCurrentIndex(model->index(-1, -1));
	updateToolButtonStates();
	relayout();
}

void PaymentView::statsItems()
{
	if(!statsView)
		statsView = new StatsView(model->stat);
	statsView->activate();
}

void PaymentView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model->sort(logicalIndex, order);
}

void PaymentView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
