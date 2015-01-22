/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_inventoriesview.h"
#include "butler_editinventoryview.h"

SCC TidContext = "InventoriesView";

SCC TidInventoriesWindowTitle = QT_TRANSLATE_NOOP("InventoriesView", "Inventory list");
SCC TidAddButton = QT_TRANSLATE_NOOP("InventoriesView", "Add new inventory");
SCC TidEditButton = QT_TRANSLATE_NOOP("InventoriesView", "Edit inventory");
SCC TidDelButton = QT_TRANSLATE_NOOP("InventoriesView", "Delete inventory");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("InventoriesView", "Refresh inventory list");

InventoriesView::InventoriesView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(inventoriesModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newInventoryView(NULL),
	editInventoryView(NULL)
{
	setWindowIcon(QIcon(Path::icon("inventory.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Inventory::LastModified);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newInventory()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editInventory()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delInventory()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

InventoriesView::~InventoriesView()
{
	delete newInventoryView;
	delete editInventoryView;
}

void InventoriesView::retranslate()
{
	QString titlePrefix(dbname == "localdb" ? "" : dbname + " :: ");
	setWindowTitle(titlePrefix + tr(TidInventoriesWindowTitle));
	relayout();
}

void InventoriesView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void InventoriesView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Inventory::Comment, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Inventory::Comment, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void InventoriesView::updateToolButtonStates()
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

void InventoriesView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void InventoriesView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void InventoriesView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void InventoriesView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void InventoriesView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void InventoriesView::loadState()
{
	QString prefix("InventoriesView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name;
	name <<= settings.value(prefix + "/currentitem", "");
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.inventorySet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editInventoryView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void InventoriesView::saveState()
{
	QString prefix("InventoriesView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.inventory(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editInventoryView);
}

void InventoriesView::newInventory()
{
	if(!newInventoryView)
		newInventoryView = new EditInventoryView(dbname);

	newInventoryView->activate();
}

void InventoriesView::editInventory()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select inventory first."));
		return;
	}

	if(!editInventoryView)
		editInventoryView = new EditInventoryView(dbname);

	editInventoryView->setCursor(tableView.currentIndex());
	editInventoryView->activate();
}

void InventoriesView::delInventory()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select inventory first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Inventory & inventory = model.inventory(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a inventory"),
			tr("Shall we delete this inventory: ") + inventory.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void InventoriesView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.inventory(tableView.currentIndex().row()).name;

	model.query();

	if(model.inventorySet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void InventoriesView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void InventoriesView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
