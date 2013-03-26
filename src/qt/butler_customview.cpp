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
#include "butler_config.h"

#include "butler_application.h"

/*
QPixmap pixmap("image.jpg");
QPalette palette;    
QPushButton *button= new QPushButton(this);
palette.setBrush(button->backgroundRole(), QBrush(pixmap));

button->setFlat(true);
button->setAutoFillBackground(true);    
button->setPalette(palette);
*/

CustomView::CustomView(const QString & dbname, bool selfDestruct, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(customModel(dbname)),
	selfDestruct(selfDestruct),
	accountingView(NULL),
	editItemView(NULL),
	queryOptsView(NULL),
	editWareView(NULL)
{
	setWindowTitle(tr("User query result"));

	QHBoxLayout * cLayout = new QHBoxLayout;
	QPushButton * button;
	
	button = new QPushButton(QIcon(Path::icon("add.png")), tr("&Accounting"));
	connect(button, SIGNAL(clicked()), this, SLOT(openAccountingView()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("edit.png")), tr("&Edit"));
	connect(button, SIGNAL(clicked()), this, SLOT(editItem()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("delete.png")), tr("&Delete"));
	connect(button, SIGNAL(clicked()), this, SLOT(delItem()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("query.png")), tr("&Filter"));
	connect(button, SIGNAL(clicked()), this, SLOT(filterItems()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("ware.png")), tr("Edit"));
	connect(button, SIGNAL(clicked()), this, SLOT(editWare()));
	cLayout->addWidget(button);

	/* query result list */
	queryView = new QTableView;
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
			Item::OnStock, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			Item::Comment, QHeaderView::Stretch);
	queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
	queryView->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(queryView->horizontalHeader(),
			SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
			this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder))
			);
	
	queryView->hideColumn(Item::Bought);

	/* statistics in grid layout */
	QGridLayout *statGrid = new QGridLayout;
	QLabel *label;

	label = new QLabel(tr("Number of queried items : "));
	statGrid->addWidget(label, 0, 0, 1, 1);
	itemCountLabel = new QLabel;
	statGrid->addWidget(itemCountLabel, 0, 1, 1, 1);
	label = new QLabel(tr("Minimal unit price : "));
	statGrid->addWidget(label, 0, 2, 1, 1);
	minUnitPriceLabel = new QLabel;
	statGrid->addWidget(minUnitPriceLabel, 0, 3, 1, 1);
	label = new QLabel(tr("Maximal unit price : "));
	statGrid->addWidget(label, 0, 4, 1, 1);
	maxUnitPriceLabel = new QLabel;
	statGrid->addWidget(maxUnitPriceLabel, 0, 5, 1, 1);


	label = new QLabel(tr("Summary of queried quantites : "));
	statGrid->addWidget(label, 1, 0, 1, 1);
	itemSumQuantityLabel = new QLabel;
	statGrid->addWidget(itemSumQuantityLabel, 1, 1, 1, 1);
	label = new QLabel(tr("Summary of paid prices : "));
	statGrid->addWidget(label, 1, 2, 1, 1);
	itemSumPriceLabel = new QLabel;
	statGrid->addWidget(itemSumPriceLabel, 1, 3, 1, 1);
	label = new QLabel(tr("Avergae unit price : "));
	statGrid->addWidget(label, 1, 4, 1, 1);
	avgUnitPriceLabel = new QLabel;
	statGrid->addWidget(avgUnitPriceLabel, 1, 5, 1, 1);

	/* making the window layouting */
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(cLayout);
	layout->addLayout(statGrid);
	layout->addWidget(&queryView);

	setLayout(layout);
	queryView.enablePanning();

	/* restore last state */
	loadState();
}

CustomView::~CustomView()
{
}

void CustomView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);

	QueriesModel & qm = queriesModel(dbname);
	if(qm.rowCount())
		model->opts = qm.query(0);
	model->opts.name = "default";

	model->query();
	updateStatistics();
	
	QSettings settings;

	QDateTime uploaded = settings.value("customview/currentitem", "").toDateTime();
	queryView->selectRow(model->index(uploaded));

	if(settings.value("customview/edititemview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void CustomView::closeEvent(QCloseEvent *event)
{
	if(!selfDestruct)
		saveState();
	else
		deleteLater();

	PannView::closeEvent(event);
}

void CustomView::loadState()
{
	QSettings settings;
	QPoint pos = settings.value("customview/position", QPoint()).toPoint();
	QSize size = settings.value("customview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void CustomView::saveState()
{
	QSettings settings;
	settings.setValue("customview/position", pos());
	settings.setValue("customview/size", size());

	QString uploaded;
	if(queryView->currentIndex().isValid()){
		const Item &item = model->item(queryView->currentIndex().row());
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

	if(!editItemView)
		editItemView = new EditItemView(dbname, *model);

	connect(editItemView, SIGNAL(finished(int)), this, SLOT(finishedEditItem(int)));
	editItemView->setCursor(queryView->currentIndex());
	editItemView->activate();
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
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select an item first!"));
		return;
	}

	WaresModel & wm = waresModel(dbname);
	if(!editWareView)
		editWareView = new EditWareView(dbname);

	const Item &item = model->item(queryView->currentIndex().row());

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
	updateStatistics();
}

void CustomView::updateStatistics()
{
	itemCountLabel->setNum((int)model->stat.itemCount);
	itemSumQuantityLabel->setNum(model->stat.sumQuantity);
	itemSumPriceLabel->setNum((int)model->stat.sumPrice);
	avgUnitPriceLabel->setNum(model->stat.avgPrice);
	minUnitPriceLabel->setNum(model->stat.cheapestUnitPrice);
	maxUnitPriceLabel->setNum(model->stat.mostExpUnitPrice);
}

void CustomView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model->sort(logicalIndex, order == Qt::AscendingOrder);
}
