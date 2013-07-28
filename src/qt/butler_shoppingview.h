/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPPINGVIEW_H
#define BUTLER_SHOPPINGVIEW_H

#include <QSqlTableModel>

#include <butler_pannview.h>
#include "butler_databases.h"

/*forwards*/
class NewItemView;
class EditItemView;
class QueryOptionsView;
class BuyItemView;
class TagFilterView;

class ShoppingView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	ShoppingView(const QString & dbname, QWidget * parent = 0);
	virtual ~ShoppingView();

	virtual void loadState();
	virtual void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

private slots:
	void newItem();
	void finishedNewItem(int);
	void editItem();
	void delItem();
	void buyItem();
	void finishedBuyItem(int price);
	void filterItems();
	void filterAcceptedSlot();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

public:
	const QString dbname;
private:
	ShoppingModel & model;

	QComboBox *partnerBox;
	QTableView partnerTableView;
	QTableView queryView;
	QSqlTableModel *queryTable;

	NewItemView *newItemView;
	BuyItemView *buyItemView;
	TagFilterView *tagFilterView;
};

#endif
