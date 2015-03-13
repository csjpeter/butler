/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPPINGMODEL_H
#define BUTLER_SHOPPINGMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include "butler_itemsmodel.h"

#include <butler_dbclasses.h>

class Item;

class ShoppingModel : public ItemsModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;
public:
	ShoppingModel(ItemDb & db, const WaresModel & wmodel);
	virtual ~ShoppingModel();

	virtual Qt::ItemFlags flags(const QModelIndex & index) const NO_FCLOG;

	void query();
	void buy(unsigned itemRow, Item & modified);
	virtual void update(int row, Item & modified);
	virtual bool queryFilter(const Item & modified);

};

#endif
