/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_STOCKVIEW_H
#define BUTLER_STOCKVIEW_H

#include <butler_pannview.h>
#include <butler_pannable.h>

#include <butler_query.h>

#include "butler_databases.h"

/*forwards*/
class QLabel;
class QTableView;
class QSqlTableModel;

class EditItemView;
class QueryOptionsView;
class ShoppingView;
class AccountingView;
class TagFilterView;

class StockView : public PannView
{
private:
	Q_OBJECT

public:
	StockView(const QString & dbname, QWidget * parent = 0);
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
	const QString & dbname;
	StockModel & model;

	Pannable<QTableView> queryView;

	EditItemView *editItemView;
	AccountingView *accountingView;
	TagFilterView *tagFilterView;
};

#endif
