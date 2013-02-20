/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

