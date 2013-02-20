/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shoppingview.h"
#include "butler_newitemview.h"
#include "butler_edititemview.h"
#include "butler_buyitemview.h"
#include "butler_queryoptionsview.h"
#include "butler_tagfilterview.h"
#include "butler_shopsmodel.h"

#include "butler_application.h"

#include "butler_db.h"

namespace Butler {

	ShoppingView::ShoppingView(QWidget *parent) :
		QWidget(parent),
		newItemView(NULL),
		editItemView(NULL),
		buyItemView(NULL),
		tagFilterView(NULL)
	{
		/* action toolbar */
		actionTB = new QToolBar(tr("Action toolbar"));

		/* actions */
		newAct = new QAction(QIcon(ICONS_PATH "add.png"), tr("&New"), this);
		newAct->setShortcut(tr("N"));
		newAct->setToolTip(tr("Add new item to buy"));
		connect(newAct, SIGNAL(triggered()), this, SLOT(newItem()));

		editAct = new QAction(QIcon(ICONS_PATH "edit.png"), tr("&Edit"), this);
		editAct->setShortcut(tr("E"));
		editAct->setToolTip(tr("Edit item details"));
		connect(editAct, SIGNAL(triggered()), this, SLOT(editItem()));

		delAct = new QAction(QIcon(ICONS_PATH "delete.png"), tr("&Delete"), this);
		delAct->setShortcut(tr("D"));
		delAct->setToolTip(tr("Delete item from shopping list"));
		connect(delAct, SIGNAL(triggered()), this, SLOT(delItem()));

		boughtAct = new QAction(QIcon(ICONS_PATH "buy.png"), tr("&Bought"), this);
		boughtAct->setShortcut(tr("B"));
		boughtAct->setToolTip(tr("Mark item as bought"));
		connect(boughtAct, SIGNAL(triggered()), this, SLOT(buyItem()));

		filterAct = new QAction(QIcon(ICONS_PATH "tag.png"), tr("&Filter"), this);
		filterAct->setShortcut(tr("F"));
		filterAct->setToolTip(tr("Filter by tags"));
		connect(filterAct, SIGNAL(triggered()), this, SLOT(filterItems()));

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

		boughtTBtn = new QToolButton(actionTB);
		actionTB->addWidget(boughtTBtn);
		boughtTBtn->setDefaultAction(boughtAct);

		filterTBtn = new QToolButton(actionTB);
		actionTB->addWidget(filterTBtn);
		filterTBtn->setDefaultAction(filterAct);

		/* shop selector */
		QGridLayout *shopLayout = new QGridLayout();

		QLabel *label = new QLabel(tr("Shop (place of buy):"));
		shopLayout->addWidget(label, 0, 0);

		shopBox = new QComboBox;
		shopBox->setModel(&ShopsModel::instance());
		shopBox->setModelColumn(Shop::Name);
		shopLayout->addWidget(shopBox, 0, 1);

		/* query result list */
		queryView = new QTableView();
		queryView->setModel(&model);
		queryView->verticalHeader()->hide();
		queryView->horizontalHeader()->setMovable(true);
		queryView->horizontalHeader()->setSortIndicatorShown(true);
		queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
		queryView->horizontalHeader()->setResizeMode(
				Item::Name, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				Item::Category, QHeaderView::ResizeToContents);
		queryView->horizontalHeader()->setResizeMode(
				Item::Quantity, QHeaderView::ResizeToContents);
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

		queryView->hideColumn(Item::Uploaded);
		queryView->hideColumn(Item::Price);
		queryView->hideColumn(Item::Shop);
		queryView->hideColumn(Item::OnStock);
		queryView->hideColumn(Item::Purchased);

		/* making the window layouting */
		QVBoxLayout *layout = new QVBoxLayout;
//		QHBoxLayout *layout = new QHBoxLayout;
		layout->setContentsMargins(0,0,0,0);
		setLayout(layout);
//		actionTB->setOrientation(Qt::Vertical);
//		actionTB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
		actionTB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
		layout->addWidget(actionTB);
		layout->addLayout(shopLayout);
		layout->addWidget(queryView);

		/* restore last state */
		loadState();
	}

	ShoppingView::~ShoppingView()
	{
	}

	void ShoppingView::showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
		if(!model.query()){
			QMessageBox(	QMessageBox::Warning,
					tr("Querying the list of items failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}

		QSettings settings(this);

		QDateTime uploaded = settings.value("shoppingview/currentitem", "").toDateTime();
		queryView->selectRow(model.index(uploaded));

		if(settings.value("shoppingview/edititemview", false).toBool())
			QTimer::singleShot(0, this, SLOT(editItem()));
	}

	void ShoppingView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QWidget::closeEvent(event);
	}

	void ShoppingView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("shoppingview/position", QPoint()).toPoint();
		QSize size = settings.value("shoppingview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void ShoppingView::saveState()
	{
		QSettings settings(this);
		settings.setValue("shoppingview/position", pos());
		settings.setValue("shoppingview/size", size());

		QString uploaded;
		if(queryView->currentIndex().isValid()){
			const Item &item = model.item(queryView->currentIndex().row());
			uploaded = item.uploaded.toString(Qt::ISODate);
		}
		settings.setValue("shoppingview/currentitem", uploaded);

		settings.setValue("shoppingview/edititemview",
				editItemView != NULL && editItemView->isVisible());
	}

	void ShoppingView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
	{
		model.sort(logicalIndex, order == Qt::AscendingOrder);
	}

	void ShoppingView::newItem()
	{
		if(!newItemView)
			newItemView = new NewItemView(this, model);

		connect(newItemView, SIGNAL(finished(int)), this, SLOT(finishedNewItem(int)));
		newItemView->show();
	}

	void ShoppingView::finishedNewItem(int res)
	{
		(void)res;

		newItemView->disconnect(this, SLOT(finishedNewItem(int)));
	}

	void ShoppingView::editItem()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select item first!"));
			return;
		}

		if(!editItemView)
			editItemView = new EditItemView(this, model);

		connect(editItemView, SIGNAL(finished(int)), this, SLOT(finishedEditItem(int)));
		editItemView->setCursor(queryView->currentIndex());
		editItemView->show();
	}

	void ShoppingView::finishedEditItem(int res)
	{
		Q_UNUSED(res);

		editItemView->disconnect(this, SLOT(finishedEditItem(int)));
	}

	void ShoppingView::delItem()
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

	void ShoppingView::buyItem()
	{
		if(!queryView->currentIndex().isValid()){
			QMessageBox::information(this, tr("Information"),
					tr("Please select item first!"));
			return;
		}
		
		if(shopBox->currentIndex() == -1){
			QMessageBox::information(this, tr("Information"),
					tr("Please select the shop where you are!"));
			return;
		}

		if(!buyItemView){
			buyItemView = new BuyItemView(this, model);
			buyItemView->setWindowTitle(tr("Mark item as bought"));
			buyItemView->setModal(true);
			connect(buyItemView, SIGNAL(finished(int)), this, SLOT(finishedBuyItem(int)));
		}
		buyItemView->setItem(queryView->currentIndex().row(), shopBox->currentIndex());
		buyItemView->show();
	}

	void ShoppingView::finishedBuyItem(int price)
	{
		Q_UNUSED(price);

		buyItemView->disconnect(this, SLOT(finishedBuyItem(int)));
	}

	void ShoppingView::filterItems()
	{
		if(!tagFilterView){
			tagFilterView = new TagFilterView(model.queryTagNames);
			tagFilterView->setModal(true);
/*			tagFilterView->setWindowModality(Qt::ApplicationModal);*/
			tagFilterView->setWindowTitle(tr("Tag filter view"));
			connect(tagFilterView, SIGNAL(accepted()),
					this, SLOT(filterAcceptedSlot()));
		}
		tagFilterView->show();
	}
	
	void ShoppingView::filterAcceptedSlot()
	{
		if(!model.query()){
			QMessageBox(	QMessageBox::Warning,
					tr("Querying the list of items failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}
	}
}


