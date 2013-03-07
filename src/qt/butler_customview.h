/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CUSTOMVIEW_H
#define BUTLER_CUSTOMVIEW_H

#include <QWidget>

#include <butler_query.h>

#include "butler_custommodel.h"

#include <butler_db.h>

/*forwards*/
class QAction;
class QToolButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;

class AccountingView;
class EditItemView;
class QueryOptionsView;
class EditWareView;

class CustomView : public QWidget
{
private:
	Q_OBJECT

public:
	CustomView(Db & db, QWidget *parent = 0);
	~CustomView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();
	
	void updateStatistics();

private slots:
	void editItem();
	void delItem();
	void openAccountingView();
	void finishedEditItem(int);
	void filterItems();
	void filterAcceptedSlot();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void editWare();
	void finishedEditWare(int);

private:
	Db & db;
	CustomModel model;

	QTableView *queryView;

	QToolBar *actionTB;

	QToolButton *editTBtn;
	QToolButton *delTBtn;
	QToolButton *accountingTBtn;
	QToolButton *filterTBtn;
	QToolButton *wareEditTBtn;

	QAction *editAct;
	QAction *delAct;
	QAction *accountingAct;
	QAction *filterAct;
	QAction *wareEditAct;

	QLabel *itemCountLabel;
	QLabel *itemSumQuantityLabel;
	QLabel *itemSumPriceLabel;
	QLabel *avgUnitPriceLabel;
	QLabel *minUnitPriceLabel;
	QLabel *maxUnitPriceLabel;

	AccountingView *accountingView;
	EditItemView *editItemView;
	QueryOptionsView *queryOptsView;
	EditWareView *editWareView;
};

#endif
