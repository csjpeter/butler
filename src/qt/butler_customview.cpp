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

CustomView::CustomView(const QString & dbname, bool selfDestruct, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(customModel(dbname)),
	selfDestruct(selfDestruct),
	toolBar(this),
	addButton(QIcon(Path::icon("add.png")), QKeySequence(Qt::Key_F1)),
	editButton(QIcon(Path::icon("edit.png")), QKeySequence(Qt::Key_F2)),
	delButton(QIcon(Path::icon("delete.png")), QKeySequence(Qt::Key_F3)),
	shoppigButton(QIcon(Path::icon("shopping.png")), QKeySequence(Qt::Key_F4)),
	filterButton(QIcon(Path::icon("query.png")), QKeySequence(Qt::Key_F5)),
	statsButton(QIcon(Path::icon("statistics.png")), QKeySequence(Qt::Key_F6)),
	accountingView(NULL),
	editItemView(NULL),
	queryOptsView(NULL),
	editWareView(NULL),
	statsView(NULL)
{
	setWindowTitle(tr("User query result"));
	setWindowIcon(QIcon(Path::icon("list.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Item::Bought);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(openAccountingView()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editItem()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delItem()));
	connect(&shoppigButton, SIGNAL(clicked()), this, SLOT(shoppingItem()));
	connect(&filterButton, SIGNAL(clicked()), this, SLOT(filterItems()));
	connect(&statsButton, SIGNAL(clicked()), this, SLOT(statsItems()));
//	connect(&editWareButton, SIGNAL(clicked()), this, SLOT(editWare()));

	loadState();
	retranslate();
}

CustomView::~CustomView()
{
}

void CustomView::retranslate()
{
	setWindowTitle(qApp->translate(
			  "", TidUserQueryWindowTitle, 0, QCoreApplication::UnicodeUTF8));
	addButton.setText(qtTrId(TidAddItemButtonLabel));
	editButton.setText(qtTrId(TidEditItemButtonLabel));
	delButton.setText(qtTrId(TidDeleteItemButtonLabel));
	shoppigButton.setText(qtTrId(TidShoppingItemButtonLabel));
	filterButton.setText(qtTrId(TidFilterItemButtonLabel));
	statsButton.setText(qtTrId(TidStatsItemButtonLabel));

	relayout();
}

enum class ViewState {
	Wide,
	Medium,
	Narrow
};

void CustomView::applyLayout()
{
	delete layout();

	HLayout * toolLayout = new HLayout;
	toolLayout->addWidget(&addButton);
	toolLayout->addWidget(&editButton);
	toolLayout->addWidget(&delButton);
	toolLayout->addWidget(&shoppigButton);
	toolLayout->addWidget(&filterButton);
	toolLayout->addWidget(&statsButton);
	toolLayout->addStretch();
	toolBar.setLayout(toolLayout);

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&toolBar);
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void CustomView::relayout()
{
	ViewState newState = ViewState::Wide;
	QSize newSize;

	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::ResizeToContents);

	switch(newState) {
		case ViewState::Wide :
			addButton.wideLayout();
			editButton.wideLayout();
			delButton.wideLayout();
			shoppigButton.wideLayout();
			filterButton.wideLayout();
			statsButton.wideLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Medium :
			addButton.mediumLayout();
			editButton.mediumLayout();
			delButton.mediumLayout();
			shoppigButton.mediumLayout();
			filterButton.mediumLayout();
			statsButton.mediumLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
		case ViewState::Narrow :
			addButton.narrowLayout();
			editButton.narrowLayout();
			delButton.narrowLayout();
			shoppigButton.narrowLayout();
			filterButton.narrowLayout();
			statsButton.narrowLayout();
			applyLayout();
			newSize = sizeHint();
			if(newSize.width() <= width())
				break;
			// falling back to a smaller size
	}
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

	QueriesModel & qm = queriesModel(dbname);
	if(qm.rowCount())
		model->opts = qm.query(0);
	model->opts.name = "default";

	model->query();
	
	QSettings settings;
	QString className = metaObject()->className();

	QDateTime uploaded = settings.value(className + "/currentitem", "").toDateTime();
	tableView.selectRow(model->index(uploaded));

	if(settings.value(className + "/edititemview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void CustomView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
	if(selfDestruct)
		deleteLater();
}

void CustomView::saveState()
{
	QString className = metaObject()->className();

	QSettings settings;
	QString uploaded;
	if(tableView.currentIndex().isValid()){
		const Item &item = model->item(tableView.currentIndex().row());
		uploaded = item.uploaded.toString(Qt::ISODate);
	}
	settings.setValue(className + "/currentitem", uploaded);

	settings.setValue(className + "/edititemview",
			editItemView != NULL && editItemView->isVisible());
}

void CustomView::editItem()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first!"));
		return;
	}

	if(!editItemView)
		editItemView = new AccountingView(dbname, *model);

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

void CustomView::openAccountingView()
{
	if(!accountingView)
		accountingView = new AccountingView(dbname, *model);
	accountingView->activate();
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

	connect(editWareView, SIGNAL(finished(int)), this, SLOT(finishedEditWare(int)));
	editWareView->setCursor(wm.index(wm.index(item.name), 0));
	editWareView->activate();
}

void CustomView::finishedEditWare(int res)
{
	Q_UNUSED(res);

	editWareView->disconnect(this, SLOT(finishedEditWare(int)));
}

void CustomView::filterItems()
{
	if(!queryOptsView){
		queryOptsView = new QueryOptionsView(dbname, model->opts);
		connect(queryOptsView, SIGNAL(accepted()), this, SLOT(filterAcceptedSlot()));
		QueriesModel & qm = queriesModel(dbname);
		if(qm.rowCount())
			model->opts = qm.query(0);
		model->opts.name = "default";
	}
	queryOptsView->activate();
}

void CustomView::filterAcceptedSlot()
{
	QueriesModel & qm = queriesModel(dbname);
	if(qm.rowCount()) {
		model->opts.name = "default";
		qm.update(0, model->opts);
	} else {
		model->opts.name = "default";
		qm.addNew(model->opts);
	}

	model->query();
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
