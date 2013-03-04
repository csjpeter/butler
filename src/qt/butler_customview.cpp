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

#include "butler_application.h"

#include "butler_db.h"

namespace Butler {

	CustomView::CustomView(QWidget *parent) :
		QWidget(parent),
		accountingView(NULL),
		editItemView(NULL),
		queryOptsView(NULL),
		editWareView(NULL)
	{
		/* action toolbar */
		actionTB = new QToolBar(tr("Action toolbar"));

		/* actions */
		accountingAct = new QAction(
				QIcon(ICONS_PATH "add.png"), tr("&Accounting"), this);
		accountingAct->setShortcut(tr("A"));
		accountingAct->setToolTip(tr("Accounting"));
		connect(accountingAct, SIGNAL(triggered()), this, SLOT(openAccountingView()));

		editAct = new QAction(QIcon(ICONS_PATH "edit.png"), tr("&Edit"), this);
		editAct->setShortcut(tr("E"));
		editAct->setToolTip(tr("Edit item details"));
		connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));

		delAct = new QAction(QIcon(ICONS_PATH "delete.png"), tr("&Delete"), this);
		delAct->setShortcut(tr("D"));
		delAct->setToolTip(tr("Delete item"));
		connect(delAct, SIGNAL(triggered()), this, SLOT(delItem()));

		filterAct = new QAction(QIcon(ICONS_PATH "query.png"), tr("&Filter"), this);
		filterAct->setShortcut(tr("F"));
		filterAct->setToolTip(tr("Filter by tags"));
		connect(filterAct, SIGNAL(triggered()), this, SLOT(filterItems()));

		wareEditAct = new QAction(QIcon(ICONS_PATH "ware.png"), tr("Edit"), this);
		wareEditAct->setShortcut(tr("W"));
		wareEditAct->setToolTip(tr("Edit ware details"));
		connect(wareEditAct, SIGNAL(triggered()), this, SLOT(editWare()));

		/* upload bills */
		accountingTBtn = new QToolButton(actionTB);
		actionTB->addWidget(accountingTBtn);
		accountingTBtn->setDefaultAction(accountingAct);

		/* tool buttons */
		editTBtn = new QToolButton(actionTB);
		actionTB->addWidget(editTBtn);
		editTBtn->setDefaultAction(editAct);

		/* delete button */
		delTBtn = new QToolButton(actionTB);
		actionTB->addWidget(delTBtn);
		delTBtn->setDefaultAction(delAct);

		/* adjust query details */
		filterTBtn = new QToolButton(actionTB);
		actionTB->addWidget(filterTBtn);
		filterTBtn->setDefaultAction(filterAct);

		/* edit ware details */
		wareEditTBtn = new QToolButton(actionTB);
		actionTB->addWidget(wareEditTBtn);
		wareEditTBtn->setDefaultAction(wareEditAct);

		/* query result list */
		queryView = new QTableView();
		queryView->setModel(&model);
//		queryView->verticalHeader()->hide();
		queryView->horizontalHeader()->setSortIndicatorShown(true);
		queryView->horizontalHeader()->setMovable(true);
		queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
		queryView->horizontalHeader()->setResizeMode(
				Item::Uploaded, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				Item::Purchased, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				Item::OnStock, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::Stretch);
		queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
		queryView->setSelectionMode(QAbstractItemView::SingleSelection);
		queryView->setBackgroundRole(QPalette::AlternateBase);
		queryView->setBackgroundRole(QPalette::Window);
		connect(queryView->horizontalHeader(),
				SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
				this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder))
				);
		
		queryView->hideColumn(Item::Bought);

		/* statistics in grid layout */
		QGridLayout *statGrid = new QGridLayout();
		QLabel *label;

		label = new QLabel(tr("Number of queried items : "));
		statGrid->addWidget(label, 0, 0, 1, 1);
		itemCountLabel = new QLabel();
		statGrid->addWidget(itemCountLabel, 0, 1, 1, 1);
		label = new QLabel(tr("Minimal unit price : "));
		statGrid->addWidget(label, 0, 2, 1, 1);
		minUnitPriceLabel = new QLabel();
		statGrid->addWidget(minUnitPriceLabel, 0, 3, 1, 1);
		label = new QLabel(tr("Maximal unit price : "));
		statGrid->addWidget(label, 0, 4, 1, 1);
		maxUnitPriceLabel = new QLabel();
		statGrid->addWidget(maxUnitPriceLabel, 0, 5, 1, 1);


		label = new QLabel(tr("Summary of queried quantites : "));
		statGrid->addWidget(label, 1, 0, 1, 1);
		itemSumQuantityLabel = new QLabel();
		statGrid->addWidget(itemSumQuantityLabel, 1, 1, 1, 1);
		label = new QLabel(tr("Summary of paid prices : "));
		statGrid->addWidget(label, 1, 2, 1, 1);
		itemSumPriceLabel = new QLabel();
		statGrid->addWidget(itemSumPriceLabel, 1, 3, 1, 1);
		label = new QLabel(tr("Avergae unit price : "));
		statGrid->addWidget(label, 1, 4, 1, 1);
		avgUnitPriceLabel = new QLabel();
		statGrid->addWidget(avgUnitPriceLabel, 1, 5, 1, 1);

		/* making the window layouting */
		QVBoxLayout *layout = new QVBoxLayout;
//		QHBoxLayout *layout = new QHBoxLayout;
		layout->setContentsMargins(0,0,0,0);
		setLayout(layout);
//		actionTB->setOrientation(Qt::Vertical);
//		actionTB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
		actionTB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		layout->addWidget(actionTB);
		layout->addLayout(statGrid);
		layout->addWidget(queryView);

		/* restore last state */
		loadState();
	}

	CustomView::~CustomView()
	{
	}

	void CustomView::showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);

		QuerySet qs;
		db().query().query(qs);
		if(qs.size())
			model.opts = qs.queryAt(0);
		model.opts.name = "default";

		if(!model.query()){
			QMessageBox(	QMessageBox::Warning,
					tr("Querying the list of items failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}
		updateStatistics();
		
		QSettings settings(this);

		QDateTime uploaded = settings.value("customview/currentitem", "").toDateTime();
		queryView->selectRow(model.index(uploaded));

		if(settings.value("customview/edititemview", false).toBool())
			QTimer::singleShot(0, this, SLOT(editItem()));
	}

	void CustomView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QWidget::closeEvent(event);
	}

	void CustomView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("customview/position", QPoint()).toPoint();
		QSize size = settings.value("customview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void CustomView::saveState()
	{
		QSettings settings(this);
		settings.setValue("customview/position", pos());
		settings.setValue("customview/size", size());

		QString uploaded;
		if(queryView->currentIndex().isValid()){
			const Item &item = model.item(queryView->currentIndex().row());
			uploaded = item.uploaded.toString(Qt::ISODate);
		}
		settings.setValue("customview/currentitem", uploaded);

		settings.setValue("customview/edititemview",
				editItemView != NULL && editItemView->isVisible());
	}

	void CustomView::editItem()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select item first!"));
			return;
		}

		if(!editItemView){
			editItemView = new EditItemView(this, model);
			editItemView->setModal(true);
/*			editItemView->setWindowModality(Qt::ApplicationModal);*/
			editItemView->setWindowTitle(tr("Edit item details"));
		}
		connect(editItemView, SIGNAL(finished(int)), this, SLOT(finishedEditItem(int)));
		editItemView->setCursor(queryView->currentIndex());
		editItemView->show();
	}

	void CustomView::finishedEditItem(int res)
	{
		Q_UNUSED(res);

		editItemView->disconnect(this, SLOT(finishedEditItem(int)));
	}

	void CustomView::delItem()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select item first!"));
			return;
		}

		int row = queryView->currentIndex().row();
		const Item &item = model.item(row);
		QMessageBox *msg = new QMessageBox(
				QMessageBox::Question,
				tr("Shall we delete?"),
				item.name + ", " + item.category,
				QMessageBox::Yes | QMessageBox::No,
				0, Qt::Dialog);
		if(msg->exec() == QMessageBox::Yes){
			if(!model.del(row)){
				QMessageBox(	QMessageBox::Warning,
						tr("Delete item failed"),
						model.error(),
						QMessageBox::Ok,
						0, Qt::Dialog).exec();
			}
		}

		delete msg;
	}

	void CustomView::openAccountingView()
	{
		if(!accountingView){
			accountingView = new AccountingView(this, model);
			accountingView->setModal(true);
/*			accountingView->setWindowModality(Qt::ApplicationModal);*/
			accountingView->setWindowTitle(tr("Accounting view"));
		}
		accountingView->show();
	}

	void CustomView::editWare()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select an item first!"));
			return;
		}

		WaresModel & waresModel = WaresModel::instance();
		if(!editWareView)
			editWareView = new EditWareView(this, waresModel);

		const Item &item = model.item(queryView->currentIndex().row());

		connect(editWareView, SIGNAL(finished(int)), this, SLOT(finishedEditWare(int)));
		editWareView->setCursor(waresModel.index(waresModel.index(item.name), 0));
		editWareView->show();
	}

	void CustomView::finishedEditWare(int res)
	{
		Q_UNUSED(res);

		editWareView->disconnect(this, SLOT(finishedEditWare(int)));
	}

	void CustomView::filterItems()
	{
		if(!queryOptsView){
			queryOptsView = new QueryOptionsView(model.opts);
			queryOptsView->setModal(true);
/*			queryOptsView->setWindowModality(Qt::ApplicationModal);*/
			queryOptsView->setWindowTitle(tr("Query options view"));
			connect(queryOptsView, SIGNAL(accepted()),
					this, SLOT(filterAcceptedSlot()));
			QuerySet qs;
			db().query().query(qs);
			if(qs.size())
				model.opts = qs.queryAt(0);
			model.opts.name = "default";
		}
		queryOptsView->show();
	}
	
	void CustomView::filterAcceptedSlot()
	{
		QuerySet qs;
		db().query().query(qs);
		if(qs.size()){
			model.opts.name = "default";
			db().query().update(qs.queryAt(0), model.opts);
		} else {
			model.opts.name = "default";
			db().query().insert(model.opts);
		}

		if(!model.query()){
			QMessageBox(	QMessageBox::Warning,
					tr("Querying the list of items failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}
		updateStatistics();
	}

	void CustomView::updateStatistics()
	{
		itemCountLabel->setNum((int)model.stat.itemCount);
		itemSumQuantityLabel->setNum(model.stat.sumQuantity);
		itemSumPriceLabel->setNum((int)model.stat.sumPrice);
		avgUnitPriceLabel->setNum(model.stat.avgPrice);
		minUnitPriceLabel->setNum(model.stat.cheapestUnitPrice);
		maxUnitPriceLabel->setNum(model.stat.mostExpUnitPrice);
	}

	void CustomView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
	{
		model.sort(logicalIndex, order == Qt::AscendingOrder);
	}
}

