/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPPINGMODEL_H
#define BUTLER_SHOPPINGMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include <butler_item.h>
#include <butler_item_set.h>
#include <butler_tag_set.h>

#include "butler_itemsmodel.h"

class Item;

class ShoppingModel : public ItemsModel
{
private:
	Q_OBJECT;
public:
	ShoppingModel(Db & db, const WaresModel & wmodel);
	virtual ~ShoppingModel();

	virtual Qt::ItemFlags flags(const QModelIndex & index) const
		__attribute__ ((no_instrument_function));

	void query();
	void buy(unsigned itemRow, Item &modified);
	virtual void update(int row, Item &modified);
	virtual bool queryFilter(const Item &modified);

public:
	TagNameSet queryTagNames;
};

#endif
