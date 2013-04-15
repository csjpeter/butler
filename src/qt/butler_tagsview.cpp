/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_tagsview.h"
#include "butler_newtagview.h"
#include "butler_edittagview.h"

TagsView::TagsView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(tagsModel(dbname)),
	newTagView(NULL),
	editTagView(NULL)
{
	setWindowTitle(tr("Tag editor"));

	QHBoxLayout * cLayout = new QHBoxLayout;
	QPushButton * button;

	button = new QPushButton(QIcon(Path::icon("add.png")), tr("&New"));
	connect(button, SIGNAL(clicked()), this, SLOT(newTag()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("edit.png")), tr("&Edit"));
	connect(button, SIGNAL(clicked()), this, SLOT(editTag()));
	cLayout->addWidget(button);

	button = new QPushButton(QIcon(Path::icon("delete.png")), tr("&Delete"));
	connect(button, SIGNAL(clicked()), this, SLOT(delTag()));
	cLayout->addWidget(button);

	/* query result list */
	queryView = new QTableView;
	queryView->setModel(&model);
	queryView->verticalHeader()->hide();
	queryView->horizontalHeader()->setMovable(true);
	queryView->horizontalHeader()->setSortIndicatorShown(true);
	queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	queryView->horizontalHeader()->setResizeMode(
			Tag::Name, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			Tag::Description, QHeaderView::Stretch);
	queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
	queryView->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(queryView->horizontalHeader(),
			SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
			this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder)));

//	queryView->hideColumn(Tag::Description);

	/* making the window layouting */
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(cLayout);
	layout->addWidget(&queryView);

	setLayout(layout);
}

TagsView::~TagsView()
{
	delete newTagView;
	delete editTagView;
}

void TagsView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	loadState();
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

	QString name = settings.value(prefix + "/currentitem", "").toString();
	queryView->selectRow(model.index(name));

	if(settings.value(prefix + "/edittagview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void TagsView::saveState()
{
	QString prefix("TagsView");
	PannView::saveState(prefix);
	QSettings settings;

	QString tagName;
	if(queryView->currentIndex().isValid())
		tagName = model.tag(queryView->currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", tagName);

	settings.setValue(prefix + "/edittagview",
			editTagView != NULL && editTagView->isVisible());
}

void TagsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void TagsView::newTag()
{
	if(!newTagView)
		newTagView = new NewTagView(dbname);

	connect(newTagView, SIGNAL(finished(int)), this, SLOT(finishedNewTag(int)));
	newTagView->activate();
}

void TagsView::finishedNewTag(int res)
{
	(void)res;

	newTagView->disconnect(this, SLOT(finishedNewTag(int)));
}

void TagsView::editTag()
{
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select tag first!"));
		return;
	}

	if(!editTagView)
		editTagView = new EditTagView(dbname);

	connect(editTagView, SIGNAL(finished(int)), this, SLOT(finishedEditTag(int)));
	editTagView->setCursor(queryView->currentIndex());
	editTagView->activate();
}

void TagsView::finishedEditTag(int res)
{
	Q_UNUSED(res);

	editTagView->disconnect(this, SLOT(finishedEditTag(int)));
}

void TagsView::delTag()
{
	if(!queryView->currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select tag first!"));
		return;
	}

	int row = queryView->currentIndex().row();
	const Tag &tag = model.tag(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a tag"),
			tr("Shall we delete this tag: ") + tag.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}
