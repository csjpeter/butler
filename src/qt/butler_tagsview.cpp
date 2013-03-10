/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_tagsview.h"
#include "butler_newtagview.h"
#include "butler_edittagview.h"

#include "butler_application.h"

#include "butler_db.h"

TagsView::TagsView(const QString & dbname, QWidget *parent) :
	QWidget(parent),
	dbname(dbname),
	model(tagsModel(dbname)),
	newTagView(NULL),
	editTagView(NULL)
{
	/* action toolbar */
	actionTB = new QToolBar(tr("Action toolbar"));

	/* actions */
	newAct = new QAction(QIcon(ICONS_PATH "add.png"), tr("&New"), this);
	newAct->setShortcut(tr("N"));
	newAct->setToolTip(tr("Add new tag to buy"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newTag()));

	editAct = new QAction(QIcon(ICONS_PATH "edit.png"), tr("&Edit"), this);
	editAct->setShortcut(tr("E"));
	editAct->setToolTip(tr("Edit tag details"));
	connect(editAct, SIGNAL(triggered()), this, SLOT(editTag()));

	delAct = new QAction(QIcon(ICONS_PATH "delete.png"), tr("&Delete"), this);
	delAct->setShortcut(tr("D"));
	delAct->setToolTip(tr("Delete tag from tagping list"));
	connect(delAct, SIGNAL(triggered()), this, SLOT(delTag()));

	/* tool buttons */
	newTBtn = new QToolButton(actionTB);
	actionTB->addWidget(newTBtn);
	newTBtn->setContentsMargins(20,20,20,20);
	newTBtn->setDefaultAction(newAct);

	delTBtn = new QToolButton(actionTB);
	actionTB->addWidget(delTBtn);
	delTBtn->setDefaultAction(delAct);

	editTBtn = new QToolButton(actionTB);
	actionTB->addWidget(editTBtn);
	editTBtn->setDefaultAction(editAct);

	/* query result list */
	queryView = new QTableView();
	queryView->setModel(&model);
	queryView->verticalHeader()->hide();
	queryView->horizontalHeader()->setMovable(true);
	queryView->horizontalHeader()->setSortIndicatorShown(true);
	queryView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
	queryView->horizontalHeader()->setResizeMode(
			TagsModel::Name, QHeaderView::ResizeToContents);
	queryView->horizontalHeader()->setResizeMode(
			TagsModel::Description, QHeaderView::Stretch);
	queryView->setSelectionBehavior(QAbstractItemView::SelectRows);
	queryView->setSelectionMode(QAbstractItemView::SingleSelection);
	queryView->setBackgroundRole(QPalette::AlternateBase);
	queryView->setBackgroundRole(QPalette::Window);
	connect(queryView->horizontalHeader(),
			SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
			this, SLOT(sortIndicatorChangedSlot(int, Qt::SortOrder)));

//	queryView->hideColumn(TagsModel::Description);

	/* making the window layouting */
	QVBoxLayout *layout = new QVBoxLayout;
//	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);
//	actionTB->setOrientation(Qt::Vertical);
//	actionTB->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	actionTB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout->addWidget(actionTB);
	layout->addWidget(queryView);

	/* restore last state */
	loadState();
}

TagsView::~TagsView()
{
}

void TagsView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
	model.query();
/*		QMessageBox(	QMessageBox::Warning,
				tr("Querying the list of tags failed"),
				model.error(),
				QMessageBox::Ok,
				0, Qt::Dialog).exec();
	}*/

	QSettings settings(this);

	QString name = settings.value("tagsview/currentitem", "").toString();
	queryView->selectRow(model.index(name));

	if(settings.value("tagsview/edittagview", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));
}

void TagsView::closeEvent(QCloseEvent *event)
{
	saveState();

	QWidget::closeEvent(event);
}

void TagsView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("tagsview/position", QPoint()).toPoint();
	QSize size = settings.value("tagsview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	move(pos);
}

void TagsView::saveState()
{
	QSettings settings(this);
	settings.setValue("tagsview/position", pos());
	settings.setValue("tagsview/size", size());

	QString tagName;
	if(queryView->currentIndex().isValid())
		tagName = model.tag(queryView->currentIndex().row()).name;
	settings.setValue("tagsview/currentitem", tagName);

	settings.setValue("tagsview/edittagview",
			editTagView != NULL && editTagView->isVisible());
}

void TagsView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void TagsView::newTag()
{
	if(!newTagView)
		newTagView = new NewTagView(dbname, this);

	connect(newTagView, SIGNAL(finished(int)), this, SLOT(finishedNewTag(int)));
	newTagView->show();
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
		editTagView = new EditTagView(dbname, this);

	connect(editTagView, SIGNAL(finished(int)), this, SLOT(finishedEditTag(int)));
	editTagView->setCursor(queryView->currentIndex());
	editTagView->show();
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
			tr("Shall we delete?"),
			tag.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
/*			QMessageBox(	QMessageBox::Warning,
					tr("Delete tag failed"),
					model.error(),
					QMessageBox::Ok,
					0, Qt::Dialog).exec();
		}*/
}
