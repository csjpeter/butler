/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_STOCKMODEL_H
#define BUTLER_STOCKMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include <butler_item.h>
#include <butler_item_set.h>
#include <butler_tag_set.h>
#include <butler_query.h>

#include "butler_itemsmodel.h"

class Item;

class StockModel : public ItemsModel
{
private:
	Q_OBJECT;
public:
	StockModel(Db & db);
	~StockModel();

	virtual Qt::ItemFlags flags(const QModelIndex & index) const
		__attribute__ ((no_instrument_function));

	void query();
	void addShoppingItem(int row);
	void drop(int row);
	virtual void update(int row, Item &modified);
	virtual bool queryFilter(const Item &modified);

public:
	Query opts;
};

#endif
