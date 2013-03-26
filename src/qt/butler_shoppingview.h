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
class QComboBox;
class QLabel;
class QTableView;
class QSqlTableModel;

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

	NewItemView *newItemView;
	EditItemView *editItemView;
	BuyItemView *buyItemView;
	TagFilterView *tagFilterView;
};

#endif
