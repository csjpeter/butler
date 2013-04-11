/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGSVIEW_H
#define BUTLER_TAGSVIEW_H

#include <butler_pannview.h>
#include <butler_pannable.h>

#include <butler_query.h>

#include "butler_databases.h"

/*forwards*/
class QLabel;
class QTableView;
class QSqlTableModel;

class NewTagView;
class EditTagView;

class TagsView : public PannView
{
private:
	Q_OBJECT

public:
	TagsView(const QString & dbname, QWidget * parent = 0);
	~TagsView();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void saveState();

private slots:
	void newTag();
	void finishedNewTag(int);
	void editTag();
	void finishedEditTag(int);
	void delTag();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

private:
	const QString & dbname;
	TagsModel & model;

	Pannable<QTableView> queryView;
	QSqlTableModel *queryTable;

	NewTagView *newTagView;
	EditTagView *editTagView;
};

#endif
