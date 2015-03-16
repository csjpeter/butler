/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_companyview.h"
#include "butler_editcompanyview.h"
#include "butler_config.h"

SCC TidContext = "CompanyView";

SCC TidCompanyWindowTitle = QT_TRANSLATE_NOOP("CompanyView", "Companies");
SCC TidAddButton = QT_TRANSLATE_NOOP("CompanyView", "Add new company");
SCC TidEditButton = QT_TRANSLATE_NOOP("CompanyView", "Edit company");
SCC TidDelButton = QT_TRANSLATE_NOOP("CompanyView", "Delete company");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("CompanyView", "Refresh company list");

CompanyView::CompanyView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(companyModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newCompanyView(NULL),
	editCompanyView(NULL)
{
	setWindowIcon(QIcon(Path::icon("company.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Company::LastModified);
	tableView.hideColumn(Company::Icon);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newCompany()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editCompany()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delCompany()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

CompanyView::~CompanyView()
{
	delete newCompanyView;
	delete editCompanyView;
}

void CompanyView::retranslate()
{
	QString titlePrefix(dbname == "localdb" ? "" : dbname + " :: ");
	setWindowTitle(titlePrefix + tr(TidCompanyWindowTitle));
	relayout();
}

void CompanyView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void CompanyView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Company::TaxId, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Company::TaxId, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void CompanyView::updateToolButtonStates()
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

void CompanyView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void CompanyView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void CompanyView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void CompanyView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void CompanyView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void CompanyView::loadState()
{
	QString prefix("CompanyView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editCompanyView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));

	updateToolButtonStates();
}

void CompanyView::saveState()
{
	QString prefix("CompanyView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editCompanyView);
}

void CompanyView::newCompany()
{
	if(!newCompanyView)
		newCompanyView = new EditCompanyView(dbname);

	newCompanyView->activate();
}

void CompanyView::editCompany()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select company first."));
		return;
	}

	if(!editCompanyView)
		editCompanyView = new EditCompanyView(dbname);

	editCompanyView->setCursor(tableView.currentIndex());
	editCompanyView->activate();
}

void CompanyView::delCompany()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select company first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Company & company = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a company"),
			tr("Shall we delete this company: ") + company.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void CompanyView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void CompanyView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void CompanyView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
