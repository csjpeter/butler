/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_shopsview.h"
#include "butler_newshopview.h"
#include "butler_editshopview.h"
#include "butler_config.h"

#include "butler_application.h"

ShopsView::ShopsView(const QString & dbname, QWidget *parent) :
	QWidget(parent),
	dbname(dbname),
	model(shopsModel(dbname)),
	newShopView(NULL),
	editShopView(NULL)
{
	/* action toolbar */
	actionTB = new QToolBar(tr("Action toolbar"));

	/* actions */
	newAct = new QAction(QIcon(Path::icon("add.png")), tr("&New"), this);
	newAct->setShortcut(tr("N"));
	newAct->setToolTip(tr("Add new shop to buy"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newShop()));

	editAct = new QAction(QIcon(Path::icon("edit.png")), tr("&Edit"), this);
	editAct->setShortcut(tr("E"));
	editAct->setToolTip(tr("Edit shop details"));
	connect(editAct, SIGNAL(triggered()), this, SLOT(editShop()));

	delAct = new QAction(QIcon(Path::icon("delete.png")), tr("&Delete"), this);
	delAct->setShortcut(tr("D"));
	delAct->setToolTip(tr("Delete shop from shopping list"));
	connect(delAct, SIGNAL(triggered()), this, SLOT(delShop()));

	/* tool buttons */
	newTBtn = new QToolButton(actionTB);
	actionTB->addWidget(newTBtn);
	newTBtn->setContentsMargins(20,20,20,20);
	newTBtn->setDefaultAction(newAct);

	delTBtn = new QToolButton(actionTB);
	actionTB->addWidget(delTBtn);
	delTBtn->setDefaultAction(delAct);

	editTBtn = new QToolButton(actionTB);
	actionTB->addWidget(editTBtn);
	editTBtn->setDefaultAction(editAct);

	/* query result list */
	queryView = new QTableView();
	queryView->setModel(&model);
	queryView->verticalHeader()->hide();
	queryView->horizontalHeader()->setMovable(true);
	queryView->horizontalHeader()->setSortIndicatorShown(true);
	queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	queryView->horizontalHeader()->setResizeMode(
			Shop::Name, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			Shop::City, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			Shop::Company, QHeaderView::Stretch);
	queryView->hideColumn(Shop::StoreName);
	queryView->hideColumn(Shop::Address);
	queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
	queryView->setSelectionMode(QAbstractItemView::SingleSelection);
	queryView->setBackgroundRole(QPalette::AlternateBase);
	queryView->setBackgroundRole(QPalette::Window);
	connect(queryView->horizontalHeader(),
			SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
			this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder)));

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

	scroll.enableKineticScrollFor(queryView);
}

ShopsView::~ShopsView()
{
	scroll.disableKineticScrollFor(queryView);
}

void ShopsView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);

	QSettings settings(this);

	QString name = settings.value("shopsview/currentitem", "").toString();
	queryView->selectRow(model.index(name));

	if(settings.value("shopsview/editshopview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void ShopsView::closeEvent(QCloseEvent *event)
{
	saveState();

	QWidget::closeEvent(event);
}

void ShopsView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("shopsview/position", QPoint()).toPoint();
	QSize size = settings.value("shopsview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	move(pos);
}

void ShopsView::saveState()
{
	QSettings settings(this);
	settings.setValue("shopsview/position", pos());
	settings.setValue("shopsview/size", size());

	QString shopName;
	if(queryView->currentIndex().isValid())
		shopName = model.shop(queryView->currentIndex().row()).name;
	settings.setValue("shopsview/currentitem", shopName);

	settings.setValue("shopsview/editshopview",
			editShopView != NULL && editShopView->isVisible());
}

void ShopsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void ShopsView::newShop()
{
	if(!newShopView)
		newShopView = new NewShopView(dbname, this);

	connect(newShopView, SIGNAL(finished(int)), this, SLOT(finishedNewShop(int)));
	newShopView->show();
}

void ShopsView::finishedNewShop(int res)
{
	(void)res;

	newShopView->disconnect(this, SLOT(finishedNewShop(int)));
}

void ShopsView::editShop()
{
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select shop first!"));
		return;
	}

	if(!editShopView)
		editShopView = new EditShopView(dbname, this);

	connect(editShopView, SIGNAL(finished(int)), this, SLOT(finishedEditShop(int)));
	editShopView->setCursor(queryView->currentIndex());
	editShopView->show();
}

void ShopsView::finishedEditShop(int res)
{
	Q_UNUSED(res);

	editShopView->disconnect(this, SLOT(finishedEditShop(int)));
}

void ShopsView::delShop()
{
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select shop first!"));
		return;
	}

	int row = queryView->currentIndex().row();
	const Shop &shop = model.shop(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a shop"),
			tr("Shall we delete this shop: ") + shop.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}
