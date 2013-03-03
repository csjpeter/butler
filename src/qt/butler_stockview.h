/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_STOCKVIEW_H
#define BUTLER_STOCKVIEW_H

#include <QWidget>

#include <butler_query.h>

#include "butler_stockmodel.h"

/*forwards*/
class QAction;
class QToolButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;

namespace Butler {

class NewItemView;
class EditItemView;
class QueryOptionsView;
class ShoppingView;
class AccountingView;
class TagFilterView;

class StockView : public QWidget
{
private:
	Q_OBJECT

public:
	StockView(QWidget *parent = 0);
	~StockView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

private slots:
	void editItem();
	void finishedEditItem(int);
	void dropItem();
	void openAccountingView();
	void filterItems();
	void filterAcceptedSlot();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

private:
	StockModel model;

	QTableView *queryView;

	QToolBar *actionTB;

	QToolButton *editTBtn;
	QToolButton *dropTBtn;
	QToolButton *accountingTBtn;
	QToolButton *filterTBtn;

	QAction *editAct;
	QAction *dropAct;
	QAction *accountingAct;
	QAction *filterAct;

	EditItemView *editItemView;
	AccountingView *accountingView;
	TagFilterView *tagFilterView;
};

}

#endif

