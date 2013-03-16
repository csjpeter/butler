/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_stockview.h"
#include "butler_edititemview.h"
#include "butler_shoppingview.h"
#include "butler_queryoptionsview.h"
#include "butler_accountingview.h"
#include "butler_tagfilterview.h"
#include "butler_config.h"

#include "butler_application.h"

StockView::StockView(const QString & dbname, QWidget *parent) :
	QWidget(parent),
	dbname(dbname),
	model(stockModel(dbname)),
	editItemView(NULL),
	accountingView(NULL),
	tagFilterView(NULL)
{
	/* action toolbar */
	actionTB = new QToolBar(tr("Action toolbar"));

	/* actions */
	accountingAct = new QAction(
			QIcon(Path::icon("add.png")), tr("&Accounting"), this);
	accountingAct->setShortcut(tr("A"));
	accountingAct->setToolTip(tr("Accounting"));
	connect(accountingAct, SIGNAL(triggered()), this, SLOT(openAccountingView()));

	editAct = new QAction(QIcon(Path::icon("edit.png")), tr("&Edit"), this);
	editAct->setShortcut(tr("E"));
	editAct->setToolTip(tr("Edit item details"));
	connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));

	dropAct = new QAction(QIcon(Path::icon("trash.png")), tr("&Drop"), this);
	dropAct->setShortcut(tr("D"));
	dropAct->setToolTip(tr("Drop item from stock"));
	connect(dropAct, SIGNAL(triggered()), this, SLOT(dropItem()));

	filterAct = new QAction(QIcon(Path::icon("tag.png")), tr("&Filter"), this);
	filterAct->setShortcut(tr("F"));
	filterAct->setToolTip(tr("Filter by tags"));
	connect(filterAct, SIGNAL(triggered()), this, SLOT(filterItems()));

	/* tool buttons */
	accountingTBtn = new QToolButton(actionTB);
	actionTB->addWidget(accountingTBtn);
	accountingTBtn->setDefaultAction(accountingAct);

	editTBtn = new QToolButton(actionTB);
	actionTB->addWidget(editTBtn);
	editTBtn->setDefaultAction(editAct);

	dropTBtn = new QToolButton(actionTB);
	actionTB->addWidget(dropTBtn);
	dropTBtn->setDefaultAction(dropAct);

	filterTBtn = new QToolButton(actionTB);
	actionTB->addWidget(filterTBtn);
	filterTBtn->setDefaultAction(filterAct);

	/* query result list */
	queryView = new QTableView();
	queryView->setModel(&model);
//	queryView->verticalHeader()->hide();
	queryView->horizontalHeader()->setSortIndicatorShown(true);
	queryView->horizontalHeader()->setMovable(true);
	queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	queryView->horizontalHeader()->setResizeMode(
			Item::Uploaded, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			Item::Purchased, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			Item::Comment, QHeaderView::Stretch);
	queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
	queryView->setSelectionMode(QAbstractItemView::SingleSelection);
	queryView->setBackgroundRole(QPalette::AlternateBase);
	queryView->setBackgroundRole(QPalette::Window);
//	queryView->columnMoved(ItemsModel::OnStock, 0);
	connect(queryView->horizontalHeader(),
			SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
			this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder))
			);

	queryView->hideColumn(Item::Uploaded);
	queryView->hideColumn(Item::Price);
	queryView->hideColumn(Item::Quantity);
	queryView->hideColumn(Item::Bought);
	queryView->hideColumn(Item::Shop);
	queryView->hideColumn(Item::OnStock);

/*	CheckBoxDelegate *delegate = new CheckBoxDelegate();
	queryView->setItemDelegate(delegate);
*/
	/* making the window layouting */
	QVBoxLayout *layout = new QVBoxLayout;
//	QHBoxLayout *layout = new QHBoxLayout;
	setLayout(layout);
//	actionTB->setOrientation(Qt::Vertical);
//	actionTB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	actionTB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout->addWidget(actionTB);
	layout->addWidget(queryView);

	/* restore last state */
	loadState();
}

StockView::~StockView()
{
}

void StockView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);

	QSettings settings(this);

	QDateTime uploaded = settings.value("stockview/currentitem", "").toDateTime();
	queryView->selectRow(model.index(uploaded));

	if(settings.value("stockview/edititemview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void StockView::closeEvent(QCloseEvent *event)
{
	saveState();

	QWidget::closeEvent(event);
}

void StockView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("stockview/position", QPoint()).toPoint();
	QSize size = settings.value("stockview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void StockView::saveState()
{
	QSettings settings(this);
	settings.setValue("stockview/position", pos());
	settings.setValue("stockview/size", size());

	QString uploaded;
	if(queryView->currentIndex().isValid()){
		const Item &item = model.item(queryView->currentIndex().row());
		uploaded = item.uploaded.toString(Qt::ISODate);
	}
	settings.setValue("stockview/currentitem", uploaded);

	settings.setValue("stockview/edititemview",
			editItemView != NULL && editItemView->isVisible());
}

void StockView::editItem()
{
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first!"));
		return;
	}

	if(!editItemView){
		editItemView = new EditItemView(dbname, model, this);
		editItemView->setModal(true);
/*		editItemView->setWindowModality(Qt::ApplicationModal);*/
		editItemView->setWindowTitle(tr("Edit item details"));
	}
	connect(editItemView, SIGNAL(finished(int)), this, SLOT(finishedEditItem(int)));
	editItemView->setCursor(queryView->currentIndex());
	editItemView->show();
}

void StockView::finishedEditItem(int res)
{
	Q_UNUSED(res);

	editItemView->disconnect(this, SLOT(finishedEditItem(int)));
}

void StockView::dropItem()
{
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first!"));
		return;
	}

	int row = queryView->currentIndex().row();
	const Item &item = model.item(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting an item from stock"),
			QString("Shall we add a corresponding item to the shopping list:\n") +
			item.name + (item.category.size() ? (" (" + item.category + ")") : ""),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
			0, Qt::Dialog));
	int res = msg->exec();
	if(res == QMessageBox::Yes)
		model.addShoppingItem(row);
	if(res == QMessageBox::Yes || res == QMessageBox::No)
		model.drop(row);
}

void StockView::openAccountingView()
{
	if(!accountingView){
		accountingView = new AccountingView(dbname, model, this);
		accountingView->setModal(true);
/*		accountingView->setWindowModality(Qt::ApplicationModal);*/
		accountingView->setWindowTitle(tr("Accounting view"));
	}
	accountingView->show();
}

void StockView::filterItems()
{
	if(!tagFilterView){
		tagFilterView = new TagFilterView(dbname, model.opts.withTags);
		tagFilterView->setModal(true);
/*		tagFilterView->setWindowModality(Qt::ApplicationModal);*/
		tagFilterView->setWindowTitle(tr("Tag filter view"));
		connect(tagFilterView, SIGNAL(accepted()),
				this, SLOT(filterAcceptedSlot()));
	}
	tagFilterView->show();
}

void StockView::filterAcceptedSlot()
{
	model.query();
}

void StockView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}
