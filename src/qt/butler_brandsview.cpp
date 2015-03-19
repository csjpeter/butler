/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_brandsview.h"
#include "butler_editbrandview.h"

SCC TidContext = "BrandsView";

SCC TidBrandsWindowTitle = QT_TRANSLATE_NOOP("BrandsView", "Brand list");
SCC TidAddButton = QT_TRANSLATE_NOOP("BrandsView", "Add new brand");
SCC TidEditButton = QT_TRANSLATE_NOOP("BrandsView", "Edit brand");
SCC TidDelButton = QT_TRANSLATE_NOOP("BrandsView", "Delete brand");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("BrandsView", "Refresh brand list");

BrandsView::BrandsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(brandModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newBrandView(NULL),
	editBrandView(NULL)
{
	setWindowIcon(QIcon(Path::icon("brand.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Brand::LastModified);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newBrand()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editBrand()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delBrand()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

BrandsView::~BrandsView()
{
	delete newBrandView;
	delete editBrandView;
}

void BrandsView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix = dbname.str;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidBrandsWindowTitle));
	relayout();
}

void BrandsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void BrandsView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Brand::Company, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Brand::Company, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void BrandsView::updateToolButtonStates()
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

void BrandsView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void BrandsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void BrandsView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void BrandsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void BrandsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void BrandsView::loadState()
{
	QString prefix("BrandsView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name;
	name <<= settings.value(prefix + "/currentitem", "");
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editBrandView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void BrandsView::saveState()
{
	QString prefix("BrandsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editBrandView);
}

void BrandsView::newBrand()
{
	if(!newBrandView)
		newBrandView = new EditBrandView(dbname);

	newBrandView->activate();
}

void BrandsView::editBrand()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select brand first."));
		return;
	}

	if(!editBrandView)
		editBrandView = new EditBrandView(dbname);

	editBrandView->setCursor(tableView.currentIndex());
	editBrandView->activate();
}

void BrandsView::delBrand()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select brand first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Brand & brand = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a brand"),
			tr("Shall we delete this brand: ") + brand.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void BrandsView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void BrandsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void BrandsView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
