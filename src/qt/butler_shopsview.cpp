/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shopsview.h"
#include "butler_newshopview.h"
#include "butler_editshopview.h"

#include "butler_application.h"

#include "butler_db.h"

namespace Butler {

	ShopsView::ShopsView(QWidget *parent) :
		QWidget(parent),
		model(ShopsModel::instance()),
		newShopView(NULL),
		editShopView(NULL)
	{
		/* action toolbar */
		actionTB = new QToolBar(tr("Action toolbar"));

		/* actions */
		newAct = new QAction(QIcon(ICONS_PATH "add.png"), tr("&New"), this);
		newAct->setShortcut(tr("N"));
		newAct->setToolTip(tr("Add new shop to buy"));
		connect(newAct, SIGNAL(triggered()), this, SLOT(newShop()));

		editAct = new QAction(QIcon(ICONS_PATH "edit.png"), tr("&Edit"), this);
		editAct->setShortcut(tr("E"));
		editAct->setToolTip(tr("Edit shop details"));
		connect(editAct, SIGNAL(triggered()), this, SLOT(editShop()));

		delAct = new QAction(QIcon(ICONS_PATH "delete.png"), tr("&Delete"), this);
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
//		QHBoxLayout *layout = new QHBoxLayout;
		layout->setContentsMargins(0,0,0,0);
		setLayout(layout);
//		actionTB->setOrientation(Qt::Vertical);
//		actionTB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
		actionTB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		layout->addWidget(actionTB);
		layout->addWidget(queryView);

		/* restore last state */
		loadState();
	}

	ShopsView::~ShopsView()
	{
	}

	void ShopsView::showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
		if(!model.query()){
			QMessageBox(	QMessageBox::Warning,
					tr("Querying the list of shops failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}

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
			newShopView = new NewShopView(this, model);

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
			editShopView = new EditShopView(this, model);

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
		QMessageBox *msg = new QMessageBox(
				QMessageBox::Question,
				tr("Shall we delete?"),
				shop.name,
				QMessageBox::Yes | QMessageBox::No,
				0, Qt::Dialog);
		if(msg->exec() == QMessageBox::Yes){
			if(!model.del(row)){
				QMessageBox(	QMessageBox::Warning,
						tr("Delete shop failed"),
						model.error(),
						QMessageBox::Ok,
						0, Qt::Dialog).exec();
			}
		}

		delete msg;
	}
}


