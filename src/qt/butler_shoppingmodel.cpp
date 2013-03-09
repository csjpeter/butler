/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shoppingmodel.h"
#include "butler_shopsmodel.h"

ShoppingModel::ShoppingModel() :
	ItemsModel()
{
}

ShoppingModel::~ShoppingModel()
{
}

Qt::ItemFlags ShoppingModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)items.size() && index.column() < Item::UnitPrice){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

void ShoppingModel::query()
{
	beginResetModel();
	db.item.query(queryTagNames, items);
	endResetModel();
}

void ShoppingModel::buy(unsigned itemRow, Item &modified)
{
	Item &orig = items.queryAt(itemRow);
	modified.bought = true;
	modified.onStock = true;
	if(db.item.update(orig, modified)){
		beginRemoveRows(QModelIndex(), itemRow, itemRow);
		items.removeAt(itemRow);
		endRemoveRows();
		itemChange(modified);
	}
}

void ShoppingModel::update(int row, Item &modified)
{
	if(ItemsModel::update(row, modified)){
		if(!queryFilter(modified)){
			beginRemoveRows(QModelIndex(), row, row);
			items.removeAt(row);
			endRemoveRows();
		}
	}
}

bool ShoppingModel::queryFilter(const Item &modified)
{
	return !modified.bought;
}
