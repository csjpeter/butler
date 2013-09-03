/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_waresview.h"
#include "butler_editwareview.h"
#include "butler_config.h"

SCC TidContext = "WaresView";

SCC TidWaresWindowTitle = QT_TRANSLATE_NOOP("WaresView", "Ware/Service list");
SCC TidAddButton = QT_TRANSLATE_NOOP("WaresView", "Add new ware or service");
SCC TidEditButton = QT_TRANSLATE_NOOP("WaresView", "Edit ware or service");
SCC TidDelButton = QT_TRANSLATE_NOOP("WaresView", "Delete ware or service");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("WaresView", "Refresh ware/service list");

WaresView::WaresView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(waresModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newWareView(NULL),
	editWareView(NULL)
{
	setWindowIcon(QIcon(Path::icon("ware.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Ware::LastModified);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newWare()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editWare()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delWare()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

WaresView::~WaresView()
{
	delete newWareView;
	delete editWareView;
}

void WaresView::retranslate()
{
	QString titlePrefix(dbname == "localdb" ? "" : dbname + " :: ");
	setWindowTitle(titlePrefix + tr(TidWaresWindowTitle));
	relayout();
}

void WaresView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void WaresView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setResizeMode(
				Ware::Categories, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setResizeMode(
				Ware::Categories, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void WaresView::updateToolButtonStates()
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

void WaresView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void WaresView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void WaresView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void WaresView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void WaresView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void WaresView::loadState()
{
	QString prefix("WaresView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.wareSet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editWareView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editWare()));
}

void WaresView::saveState()
{
	QString prefix("WaresView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.ware(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editWareView);
}

void WaresView::newWare()
{
	if(!newWareView)
		newWareView = new EditWareView(dbname);

	newWareView->activate();
}

void WaresView::editWare()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select ware first."));
		return;
	}

	if(!editWareView)
		editWareView = new EditWareView(dbname);

	editWareView->setCursor(tableView.currentIndex());
	editWareView->activate();
}

void WaresView::delWare()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select ware first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Ware & ware = model.ware(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a ware"),
			tr("Shall we delete this ware: ") + ware.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void WaresView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.ware(tableView.currentIndex().row()).name;

	model.query();

	if(model.wareSet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void WaresView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void WaresView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
