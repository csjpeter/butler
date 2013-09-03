/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_databasesview.h"
#include "butler_editdbdescview.h"

#include <butler_databasesmodel.h>

SCC TidContext = "DatabasesView";

SCC TidDatabasesWindowTitle = QT_TRANSLATE_NOOP("DatabasesView", "Database connections list");
SCC TidAddButton = QT_TRANSLATE_NOOP("DatabasesView", "Add new database connection");
SCC TidEditButton = QT_TRANSLATE_NOOP("DatabasesView", "Edit database connection");
SCC TidDelButton = QT_TRANSLATE_NOOP("DatabasesView", "Delete database connection");
SCC TidUseButton = QT_TRANSLATE_NOOP("DatabasesView", "Use database connection");
SCC TidCurrentDbInfo = QT_TRANSLATE_NOOP("DatabasesView", "Current database: %1");

DatabasesModel model;

DatabasesView::DatabasesView(QWidget * parent) :
	PannView(parent),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	useButton(QIcon(Path::icon("use.png")),
			TidUseButton, TidContext, QKeySequence(Qt::Key_F4)),
	newDbDescView(NULL),
	editDbDescView(NULL)
{
	setWindowIcon(QIcon(Path::icon("databases.png")));

	tableView.setModel(&model);
	tableView.hideColumn(DatabaseDescriptor::Password);
	tableView.hideColumn(DatabaseDescriptor::SavePassword);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(useButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newDbDesc()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editDbDesc()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delDbDesc()));
	connect(&useButton, SIGNAL(clicked()), this, SLOT(useDbDesc()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

DatabasesView::~DatabasesView()
{
	delete newDbDescView;
	delete editDbDescView;
}

void DatabasesView::retranslate()
{
	setWindowTitle(tr(TidDatabasesWindowTitle));
	toolBar.setInfo(tr(TidCurrentDbInfo).arg(Config::defaultDbName));
	relayout();
}

void DatabasesView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void DatabasesView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setResizeMode(
				DatabaseDescriptor::DatabaseName, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setResizeMode(
				DatabaseDescriptor::DatabaseName, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void DatabasesView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
		useButton.show();
	} else {
		editButton.hide();
		delButton.hide();
		useButton.hide();
	}
	toolBar.updateButtons();
}

void DatabasesView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void DatabasesView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void DatabasesView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void DatabasesView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void DatabasesView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void DatabasesView::loadState()
{
	QString prefix("DatabasesView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	QString dbname; dbname <<= settings.value(prefix + "/currentitem", "");
	int col; col <<= settings.value(prefix + "/currentitemCol", "");
	if(model.has(dbname))
		tableView.setCurrentIndex(model.index(model.index(dbname), col));

	if(settings.value(prefix + "/newDbDescView", false).toBool())
		QTimer::singleShot(0, this, SLOT(newDbDesc()));

	if(settings.value(prefix + "/editDbDescView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editDbDesc()));
}

void DatabasesView::saveState()
{
	QString prefix("DatabasesView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.query(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(newDbDescView);
	SAVE_VIEW_STATE(editDbDescView);
}

void DatabasesView::newDbDesc()
{
	if(!newDbDescView)
		newDbDescView = new EditDbDescView(model);

	newDbDescView->activate();
}

void DatabasesView::editDbDesc()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first."));
		return;
	}

	if(!editDbDescView)
		editDbDescView = new EditDbDescView(model);

	editDbDescView->setCursor(tableView.currentIndex());
	editDbDescView->activate();
}

void DatabasesView::delDbDesc()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select item first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const DatabaseDescriptor & dbdesc = model.query(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting an database connection"),
			tr("Shall we delete this database connection: ") + dbdesc.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void DatabasesView::useDbDesc()
{
	int row = tableView.currentIndex().row();
	try {
		companyModel(model.query(row).name);
		Config::defaultDbName = model.query(row).name;
		toolBar.setInfo(tr(TidCurrentDbInfo).arg(Config::defaultDbName));
		activeDbChanged();
	} catch (DbConnectError & e) {
		QString info = e.what();
		QMessageBox(QMessageBox::Warning, "Exception thrown", info,
			QMessageBox::Ok, 0, Qt::Dialog).exec();
	}
}

void DatabasesView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void DatabasesView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
