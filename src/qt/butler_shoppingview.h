/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPPINGVIEW_H
#define BUTLER_SHOPPINGVIEW_H

#include <QWidget>

#include <butler_query.h>

#include "butler_shoppingmodel.h"

/*forwards*/
class QAction;
class QToolButton;
class QComboBox;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;
class QComboBox;

namespace Butler {

class NewItemView;
class EditItemView;
class QueryOptionsView;
class BuyItemView;
class TagFilterView;

class ShoppingView : public QWidget
{
private:
	Q_OBJECT

public:
	ShoppingView(QWidget *parent = 0);
	~ShoppingView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

private slots:
	void newItem();
	void finishedNewItem(int);
	void editItem();
	void finishedEditItem(int);
	void delItem();
	void buyItem();
	void finishedBuyItem(int price);
	void filterItems();
	void filterAcceptedSlot();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

private:
	ShoppingModel model;

	QComboBox *shopBox;
	QTableView *queryView;
	QSqlTableModel *queryTable;

	QToolBar *actionTB;

	QToolButton *newTBtn;
	QToolButton *editTBtn;
	QToolButton *delTBtn;
	QToolButton *boughtTBtn;
	QToolButton *filterTBtn;

	QAction *newAct;
	QAction *editAct;
	QAction *delAct;
	QAction *boughtAct;
	QAction *filterAct;

	NewItemView *newItemView;
	EditItemView *editItemView;
	BuyItemView *buyItemView;
	TagFilterView *tagFilterView;
};

}

#endif


