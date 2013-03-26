/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPPINGVIEW_H
#define BUTLER_SHOPPINGVIEW_H

#include <butler_pannview.h>
#include <butler_pannable.h>

#include <butler_query.h>

#include "butler_databases.h"

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

class NewItemView;
class EditItemView;
class QueryOptionsView;
class BuyItemView;
class TagFilterView;

class ShoppingView : public PannView
{
private:
	Q_OBJECT

public:
	ShoppingView(const QString & dbname, QWidget * parent = 0);
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
	const QString & dbname;
	ShoppingModel & model;

	QComboBox *shopBox;
	Pannable<QTableView> queryView;
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

#endif
