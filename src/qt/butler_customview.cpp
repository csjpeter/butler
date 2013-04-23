/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_customview.h"
#include "butler_edititemview.h"
#include "butler_shoppingview.h"
#include "butler_queryoptionsview.h"
#include "butler_accountingview.h"
#include "butler_editwareview.h"
#include "butler_statsview.h"
#include "butler_config.h"

CustomView::CustomView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(customModel(dbname)),
	toolBar(this),
	editButton(QIcon(Path::icon("edit.png")), QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")), QKeySequence(Qt::Key_F2)),
	shoppigButton(QIcon(Path::icon("shopping.png")), QKeySequence(Qt::Key_F3)),
	statsButton(QIcon(Path::icon("statistics.png")), QKeySequence(Qt::Key_F4)),
	refreshButton(QIcon(Path::icon("refresh.png")), QKeySequence(QKeySequence::Refresh)),/*F5*/
	filterButton(QIcon(Path::icon("query.png")), QKeySequence(Qt::Key_F6)),
	accountingView(NULL),
	editItemView(NULL),
	queryOptsView(NULL),
	editWareView(NULL),
	statsView(NULL)
{
	setWindowIcon(QIcon(Path::icon("list.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Item::Bought);

	connect(&editButton, SIGNAL(clicked()), this, SLOT(editItem()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delItem()));
	connect(&shoppigButton, SIGNAL(clicked()), this, SLOT(shoppingItem()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refreshItems()));
	connect(&filterButton, SIGNAL(clicked()), this, SLOT(filterItems()));
	connect(&statsButton, SIGNAL(clicked()), this, SLOT(statsItems()));
//	connect(&editWareButton, SIGNAL(clicked()), this, SLOT(editWare()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	retranslate();
	loadState();
}

CustomView::~CustomView()
{
	delete accountingView;
	delete editItemView;
	delete queryOptsView;
	delete editWareView;
	delete statsView;
}

void CustomView::retranslate()
{
	setWindowTitle(qtTrId(TidAnaliticsWindowTitle));
	editButton.setText(qtTrId(TidEditItemButtonLabel));
	delButton.setText(qtTrId(TidDeleteItemButtonLabel));
	shoppigButton.setText(qtTrId(TidShoppingItemButtonLabel));
	refreshButton.setText(qtTrId(TidRefreshItemsButtonLabel));
	filterButton.setText(qtTrId(TidFilterItemButtonLabel));
	statsButton.setText(qtTrId(TidStatsItemButtonLabel));

	relayout();
}

void CustomView::applyLayout()
{
	delete layout();

	HLayout * toolLayout = new HLayout;
	toolLayout->addWidget(&editButton);
	toolLayout->addWidget(&delButton);
	toolLayout->addWidget(&shoppigButton);
	toolLayout->addWidget(&statsButton);
	toolLayout->addWidget(&refreshButton);
	toolLayout->addWidget(&filterButton);
	toolLayout->addStretch();
	toolBar.setLayout(toolLayout);
	setToolBar(&toolBar);

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void CustomView::relayout()
{
	LOG("relayout");
	LayoutState newState = LayoutState::Wide;
	QSize newSize;

	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::ResizeToContents);

	switch(newState) {
		case LayoutState::Expanding :
		case LayoutState::Wide :
			editButton.wideLayout();
			delButton.wideLayout();
			shoppigButton.wideLayout();
			refreshButton.wideLayout();
			filterButton.wideLayout();
			statsButton.wideLayout();
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
		case LayoutState::Medium :
			editButton.mediumLayout();
			delButton.mediumLayout();
			refreshButton.mediumLayout();
			shoppigButton.mediumLayout();
			filterButton.mediumLayout();
			statsButton.mediumLayout();
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
		case LayoutState::Narrow :
			editButton.narrowLayout();
			delButton.narrowLayout();
			shoppigButton.narrowLayout();
			refreshButton.narrowLayout();
			filterButton.narrowLayout();
			statsButton.narrowLayout();
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
	}
	updateToolButtonStates();
}

void CustomView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void CustomView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void CustomView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void CustomView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void CustomView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void CustomView::loadState()
{
	QString prefix("CustomView");
	PannView::loadState(prefix);
	QSettings settings;

	QString queryName = settings.value(prefix + "/query", "").toString();
	QueriesModel & qm = queriesModel(dbname);
	if(qm.querySet().has(queryName))
		model->opts = qm.querySet().query(queryName);
	model->query();

	tableView.loadState(prefix);

	QDateTime uploaded = settings.value(prefix + "/currentitem", "").toDateTime();
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model->itemSet().has(uploaded))
		tableView.setCurrentIndex(model->index(model->index(uploaded), col));

	if(settings.value(prefix + "/editItemView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));

	if(settings.value(prefix + "/queryOptsView", false).toBool())
		QTimer::singleShot(0, this, SLOT(filterItems()));

	updateToolButtonStates();
}

void CustomView::saveState()
{
	QString prefix("CustomView");
	PannView::saveState(prefix);
	QSettings settings;

	settings.setValue(prefix + "/query", model->opts.name);

	QString uploaded;
	if(tableView.currentIndex().isValid()){
		const Item &item = model->item(tableView.currentIndex().row());
		uploaded = item.uploaded.toString(Qt::ISODate);
	}
	settings.setValue(prefix + "/currentitem", uploaded);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editItemView);
	SAVE_VIEW_STATE(queryOptsView);
}

void CustomView::refreshItems()
{
	QDateTime uploaded;
	if(tableView.currentIndex().isValid()){
		const Item &item = model->item(tableView.currentIndex().row());
		uploaded = item.uploaded;
	}

	model->query();

	if(model->itemSet().has(uploaded))
		tableView.setCurrentIndex(model->index(model->index(uploaded), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void CustomView::editItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first!"));
		return;
	}

	if(!editItemView){
		editItemView = new AccountingView(dbname, *model);
		editItemView->setCursor(tableView.currentIndex());
		editItemView->loadState();
		editItemView->activate();
	}

	editItemView->setCursor(tableView.currentIndex());
	editItemView->activate();
}

void CustomView::delItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first!"));
		return;
	}

	int row = tableView.currentIndex().row();
	const Item &item = model->item(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting an item"),
			tr("Shall we delete this item: ") + item.name +
			(item.category.size() ? (" (" + item.category + ")") : ""),
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model->del(row);
}

void CustomView::shoppingItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first!"));
		return;
	}

	int row = tableView.currentIndex().row();
	const Item &item = model->item(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Adding to shopping list"),
			QString("Shall we add a corresponding item to the shopping list:\n") +
			item.name + (item.category.size() ? (" (" + item.category + ")") : ""),
			QMessageBox::Yes | QMessageBox::No, 0, Qt::Dialog));
	int res = msg->exec();
	if(res == QMessageBox::Yes)
		model->addShoppingItem(row);
}

void CustomView::editWare()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select an item first!"));
		return;
	}

	WaresModel & wm = waresModel(dbname);
	if(!editWareView)
		editWareView = new EditWareView(dbname);

	const Item &item = model->item(tableView.currentIndex().row());

	editWareView->setCursor(wm.index(wm.index(item.name), 0));
	editWareView->activate();
}

void CustomView::filterItems()
{
	if(!queryOptsView){
		queryOptsView = new QueryOptionsView(dbname);
		connect(queryOptsView, SIGNAL(accepted()), this, SLOT(applyNewFilter()));
	}
	queryOptsView->query = model->opts;
	queryOptsView->activate();
}

void CustomView::applyNewFilter()
{
	model->opts = queryOptsView->query;
	model->query();
	tableView.setCurrentIndex(model->index(-1, -1));
	updateToolButtonStates();
	relayout();
}

void CustomView::statsItems()
{
	if(!statsView)
		statsView = new StatsView(model->stat);
	statsView->activate();
}

void CustomView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model->sort(logicalIndex, order == Qt::AscendingOrder);
}

void CustomView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
		shoppigButton.show();
	} else {
		editButton.hide();
		delButton.hide();
		shoppigButton.hide();
	}
}

void CustomView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	LOG("current index changed");
	if(current.isValid() == previous.isValid())
		return;
	LOG("current index really changed");

	if(current.isValid()){
		if(!editButton.isVisible()){
			editButton.show();
			delButton.show();
			shoppigButton.show();
			relayout();
		}
	} else if(editButton.isVisible()) {
		editButton.hide();
		delButton.hide();
		shoppigButton.hide();
		relayout();
	}
}
