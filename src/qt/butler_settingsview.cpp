/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_settingsview.h"
#include "butler_edittagview.h"

SCC TidContext = "SettingsView";

SCC TidTagsWindowTitle = QT_TRANSLATE_NOOP("SettingsView", "Tag list");
SCC TidAddButton = QT_TRANSLATE_NOOP("SettingsView", "Add new tag");
SCC TidEditButton = QT_TRANSLATE_NOOP("SettingsView", "Edit tag");
SCC TidDelButton = QT_TRANSLATE_NOOP("SettingsView", "Delete tag");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("SettingsView", "Refresh tag list");

SettingsView::SettingsView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(tagsModel(dbname)),
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

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newTag()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editTag()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delTag()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	setupView();
	retranslate();
	loadState();
}

SettingsView::~SettingsView()
{
	delete newTagView;
	delete editTagView;
}

void SettingsView::retranslate()
{
	setWindowTitle(tr(TidTagsWindowTitle));
	relayout();
}

void SettingsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;

	setLayout(mainLayout);
}

void SettingsView::relayout()
{
	applyLayout();

	updateToolButtonStates();
}

void SettingsView::updateToolButtonStates()
{
	toolBar.updateButtons();
}

void SettingsView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void SettingsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void SettingsView::keyPressEvent(QKeyEvent * event)
{
}

void SettingsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void SettingsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void SettingsView::loadState()
{
	QString prefix("SettingsView");
	PannView::loadState(prefix);
	QSettings settings;
}

void SettingsView::saveState()
{
	QString prefix("SettingsView");
	PannView::saveState(prefix);
	QSettings settings;
}

void SettingsView::newTag()
{
	if(!newTagView)
		newTagView = new EditTagView(dbname);

	newTagView->activate();
}

void SettingsView::editTag()
{
}

void SettingsView::delTag()
{
}

void SettingsView::refresh()
{
}

void SettingsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void SettingsView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
