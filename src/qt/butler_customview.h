/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CUSTOMVIEW_H
#define BUTLER_CUSTOMVIEW_H

#include <QWidget>

#include <butler_query.h>

#include "butler_custommodel.h"

#include "butler_localdb.h"

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

class AccountingView;
class EditItemView;
class QueryOptionsView;
class EditWareView;

class CustomView : public QWidget//, public LocalDb
{
private:
	Q_OBJECT

public:
	CustomView(QWidget *parent = 0);
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

}

#endif
