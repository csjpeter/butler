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

ShopsView::ShopsView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(shopsModel(dbname)),
	newShopView(NULL),
	editShopView(NULL)
{
	setWindowTitle(tr("Shop list"));

	QHBoxLayout * cLayout = new QHBoxLayout;
	QPushButton * button;

	button = new QPushButton(QIcon(Path::icon("add.png")), tr("&New"));
	connect(button, SIGNAL(clicked()), this, SLOT(newShop()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("edit.png")), tr("&Edit"));
	connect(button, SIGNAL(clicked()), this, SLOT(editShop()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("delete.png")), tr("&Delete"));
	connect(button, SIGNAL(clicked()), this, SLOT(delShop()));
	cLayout->addWidget(button);

	/* query result list */
	queryView = new QTableView;
	queryView->setModel(&model);
	queryView->verticalHeader()->hide();
	queryView->horizontalHeader()->setMovable(true);
	queryView->horizontalHeader()->setSortIndicatorShown(true);
	queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	queryView->horizontalHeader()->setResizeMode(Shop::Name, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(Shop::City, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(Shop::Company, QHeaderView::Stretch);
	queryView->hideColumn(Shop::StoreName);
	queryView->hideColumn(Shop::Address);
	queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
	queryView->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(queryView->horizontalHeader(),
			SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
			this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder)));

	/* making the window layouting */
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(cLayout);
	layout->addWidget(&queryView);

	setLayout(layout);
	loadState();
}

ShopsView::~ShopsView()
{
	delete newShopView;
	delete editShopView;
}

void ShopsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
}

void ShopsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void ShopsView::loadState()
{
	QString prefix("ShopsView");
	PannView::loadState(prefix);
	QSettings settings;

	QString name = settings.value(prefix + "/currentitem", "").toString();
	queryView->selectRow(model.index(name));

	if(settings.value(prefix + "/editshopview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void ShopsView::saveState()
{
	QString prefix("ShopsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString shopName;
	if(queryView->currentIndex().isValid())
		shopName = model.shop(queryView->currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", shopName);

	settings.setValue(prefix + "/editshopview",
			editShopView != NULL && editShopView->isVisible());
}

void ShopsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void ShopsView::newShop()
{
	if(!newShopView)
		newShopView = new NewShopView(dbname);

	connect(newShopView, SIGNAL(finished(int)), this, SLOT(finishedNewShop(int)));
	newShopView->activate();
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
		editShopView = new EditShopView(dbname);

	connect(editShopView, SIGNAL(finished(int)), this, SLOT(finishedEditShop(int)));
	editShopView->setCursor(queryView->currentIndex());
	editShopView->activate();
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
