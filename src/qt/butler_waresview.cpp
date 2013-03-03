/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_waresview.h"
#include "butler_newwareview.h"
#include "butler_editwareview.h"

#include "butler_application.h"

#include "butler_db.h"

namespace Butler {

	WaresView::WaresView(QWidget *parent) :
		QWidget(parent),
		model(WaresModel::instance()),
		newWareView(NULL),
		editWareView(NULL)
	{
		/* action toolbar */
		actionTB = new QToolBar(tr("Action toolbar"));

		/* actions */
		newAct = new QAction(QIcon(ICONS_PATH "add.png"), tr("&New"), this);
		newAct->setShortcut(tr("N"));
		newAct->setToolTip(tr("Add new ware to buy"));
		connect(newAct, SIGNAL(triggered()), this, SLOT(newWare()));

		editAct = new QAction(QIcon(ICONS_PATH "edit.png"), tr("&Edit"), this);
		editAct->setShortcut(tr("E"));
		editAct->setToolTip(tr("Edit ware details"));
		connect(editAct, SIGNAL(triggered()), this, SLOT(editWare()));

		delAct = new QAction(QIcon(ICONS_PATH "delete.png"), tr("&Delete"), this);
		delAct->setShortcut(tr("D"));
		delAct->setToolTip(tr("Delete ware from wareping list"));
		connect(delAct, SIGNAL(triggered()), this, SLOT(delWare()));

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
				WaresModel::Name, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				WaresModel::Unit, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				WaresModel::Categories, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				WaresModel::Tags, QHeaderView::Stretch);
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

	WaresView::~WaresView()
	{
	}

	void WaresView::showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
		if(!model.query()){
			QMessageBox(	QMessageBox::Warning,
					tr("Querying the list of wares failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}

		QSettings settings(this);

		QString name = settings.value("waresview/currentitem", "").toString();
		queryView->selectRow(model.index(name));

		if(settings.value("waresview/editwareview", false).toBool())
			QTimer::singleShot(0, this, SLOT(editItem()));
	}

	void WaresView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QWidget::closeEvent(event);
	}

	void WaresView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("waresview/position", QPoint()).toPoint();
		QSize size = settings.value("waresview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void WaresView::saveState()
	{
		QSettings settings(this);
		settings.setValue("waresview/position", pos());
		settings.setValue("waresview/size", size());

		QString wareName;
		if(queryView->currentIndex().isValid())
			wareName = model.ware(queryView->currentIndex().row()).name;
		settings.setValue("waresview/currentitem", wareName);

		settings.setValue("waresview/editwareview",
				editWareView != NULL && editWareView->isVisible());
	}
	
	void WaresView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
	{
		model.sort(logicalIndex, order == Qt::AscendingOrder);
	}

	void WaresView::newWare()
	{
		if(!newWareView)
			newWareView = new NewWareView(this, model);

		connect(newWareView, SIGNAL(finished(int)), this, SLOT(finishedNewWare(int)));
		newWareView->show();
	}

	void WaresView::finishedNewWare(int res)
	{
		(void)res;

		newWareView->disconnect(this, SLOT(finishedNewWare(int)));
	}

	void WaresView::editWare()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select ware first!"));
			return;
		}

		if(!editWareView)
			editWareView = new EditWareView(this, model);

		connect(editWareView, SIGNAL(finished(int)), this, SLOT(finishedEditWare(int)));
		editWareView->setCursor(queryView->currentIndex());
		editWareView->show();
	}

	void WaresView::finishedEditWare(int res)
	{
		Q_UNUSED(res);

		editWareView->disconnect(this, SLOT(finishedEditWare(int)));
	}

	void WaresView::delWare()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select ware first!"));
			return;
		}

		int row = queryView->currentIndex().row();
		const Ware &ware = model.ware(row);
		QMessageBox *msg = new QMessageBox(
				QMessageBox::Question,
				tr("Shall we delete?"),
				ware.name,
				QMessageBox::Yes | QMessageBox::No,
				0, Qt::Dialog);
		if(msg->exec() == QMessageBox::Yes){
			if(!model.del(row)){
				QMessageBox(	QMessageBox::Warning,
						tr("Delete ware failed"),
						model.error(),
						QMessageBox::Ok,
						0, Qt::Dialog).exec();
			}
		}

		delete msg;
	}
}

