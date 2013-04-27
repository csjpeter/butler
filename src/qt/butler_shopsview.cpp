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

SCC TidContext = "PartnersView";

SCC TidPartnersWindowTitle = QT_TRANSLATE_NOOP("PartnersView", "Business partners");
SCC TidAddButton = QT_TRANSLATE_NOOP("PartnersView", "Add new partner");
SCC TidEditButton = QT_TRANSLATE_NOOP("PartnersView", "Edit partner");
SCC TidDelButton = QT_TRANSLATE_NOOP("PartnersView", "Delete partner");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("PartnersView", "Refresh partner list");

PartnersView::PartnersView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(shopsModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newShopView(NULL),
	editShopView(NULL)
{
	setWindowTitle(tr("Shop list"));

	tableView.setModel(&model);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newShop()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editShop()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delShop()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

PartnersView::~PartnersView()
{
	delete newShopView;
	delete editShopView;
}

void PartnersView::retranslate()
{
	setWindowTitle(tr(TidPartnersWindowTitle));
	relayout();
}

void PartnersView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void PartnersView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setResizeMode(
				Item::Comment, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void PartnersView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void PartnersView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void PartnersView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void PartnersView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void PartnersView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void PartnersView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void PartnersView::loadState()
{
	QString prefix("PartnersView");
	PannView::loadState(prefix);
	QSettings settings;

	QString name = settings.value(prefix + "/currentitem", "").toString();
	tableView.selectRow(model.index(name));

	if(settings.value(prefix + "/editshopview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void PartnersView::saveState()
{
	QString prefix("PartnersView");
	PannView::saveState(prefix);
	QSettings settings;

	QString shopName;
	if(tableView.currentIndex().isValid())
		shopName = model.shop(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", shopName);

	settings.setValue(prefix + "/editshopview",
			editShopView != NULL && editShopView->isVisible());
}

void PartnersView::newShop()
{
	if(!newShopView)
		newShopView = new NewShopView(dbname);

	newShopView->activate();
}

void PartnersView::editShop()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select shop first."));
		return;
	}

	if(!editShopView)
		editShopView = new EditShopView(dbname);

	editShopView->setCursor(tableView.currentIndex());
	editShopView->activate();
}

void PartnersView::delShop()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select shop first."));
		return;
	}

	int row = tableView.currentIndex().row();
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

void PartnersView::refresh()
{
	QString name;
	if(tableView.currentIndex().isValid())
		name = model.shop(tableView.currentIndex().row()).name;

	model.query();

	if(model.shopSet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}
