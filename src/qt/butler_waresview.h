/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARESVIEW_H
#define BUTLER_WARESVIEW_H

#include <butler_pannview.h>
#include <butler_pannable.h>

#include <butler_query.h>

#include "butler_databases.h"

/*forwards*/
class QLabel;
class QTableView;
class QSqlTableModel;

class NewWareView;
class EditWareView;

class WaresView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	WaresView(const QString & dbname, QWidget * parent = 0);
	virtual ~WaresView();

	virtual void loadState();
	virtual void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

private slots:
	void newWare();
	void finishedNewWare(int);
	void editWare();
	void finishedEditWare(int);
	void delWare();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

private:
	const QString & dbname;
	WaresModel & model;

	Pannable<QTableView> queryView;
	QSqlTableModel *queryTable;

	NewWareView *newWareView;
	EditWareView *editWareView;
};

#endif
