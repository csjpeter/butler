/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_tagsview.h"
#include "butler_edittagview.h"

SCC TidContext = "TagsView";

SCC TidTagsWindowTitle = QT_TRANSLATE_NOOP("TagsView", "Tag list");
SCC TidAddButton = QT_TRANSLATE_NOOP("TagsView", "Add new tag");
SCC TidEditButton = QT_TRANSLATE_NOOP("TagsView", "Edit tag");
SCC TidDelButton = QT_TRANSLATE_NOOP("TagsView", "Delete tag");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("TagsView", "Refresh tag list");

TagsView::TagsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(tagModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newTagView(NULL),
	editTagView(NULL)
{
	setWindowIcon(QIcon(Path::icon("tag.png")));

	tableView.setModel(&model);
	tableView.hideColumn(Tag::LastModified);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newTag()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editTag()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delTag()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

TagsView::~TagsView()
{
	delete newTagView;
	delete editTagView;
}

void TagsView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix = dbname.str;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(TidTagsWindowTitle));
	relayout();
}

void TagsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void TagsView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				Tag::Description, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				Tag::Description, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void TagsView::updateToolButtonStates()
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

void TagsView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void TagsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void TagsView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void TagsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void TagsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void TagsView::loadState()
{
	QString prefix("TagsView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name;
	name <<= settings.value(prefix + "/currentitem", "");
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editTagView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void TagsView::saveState()
{
	QString prefix("TagsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editTagView);
}

void TagsView::newTag()
{
	if(!newTagView)
		newTagView = new EditTagView(dbname);

	newTagView->activate();
}

void TagsView::editTag()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select tag first."));
		return;
	}

	if(!editTagView)
		editTagView = new EditTagView(dbname);

	editTagView->setCursor(tableView.currentIndex());
	editTagView->activate();
}

void TagsView::delTag()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select tag first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Tag & tag = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a tag"),
			tr("Shall we delete this tag: ") + tag.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void TagsView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void TagsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void TagsView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
