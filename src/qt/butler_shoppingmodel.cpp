/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shoppingmodel.h"
#include "butler_partnersmodel.h"

ShoppingModel::ShoppingModel(Db & db, const WaresModel & wmodel) :
	ItemsModel(db, wmodel)
{
	query();
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
	try {
		beginResetModel();
		db.item.query(queryTagNames, items);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

void ShoppingModel::buy(unsigned itemRow, Item &modified)
{
	Item &orig = items.queryAt(itemRow);
	modified.bought = true;
	modified.onStock = true;
	db.item.update(orig, modified);
	itemChange(db, modified);
}

void ShoppingModel::update(int row, Item &modified)
{
	ItemsModel::update(row, modified);
	if(queryFilter(modified))
		return;

	try {
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

bool ShoppingModel::queryFilter(const Item &modified)
{
	return !modified.bought;
}
