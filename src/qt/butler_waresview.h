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
class QAction;
class QToolButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;

class NewWareView;
class EditWareView;

class WaresView : public PannView
{
private:
	Q_OBJECT

public:
	WaresView(const QString & dbname, QWidget *parent = 0);
	~WaresView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

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

	QToolBar *actionTB;

	QToolButton *newTBtn;
	QToolButton *editTBtn;
	QToolButton *delTBtn;

	QAction *newAct;
	QAction *editAct;
	QAction *delAct;

	NewWareView *newWareView;
	EditWareView *editWareView;
};

#endif
