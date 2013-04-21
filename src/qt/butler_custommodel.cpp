/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_custommodel.h"

CustomModel::CustomModel(Db & db, const WaresModel & wmodel) :
	ItemsModel(db, wmodel)
{
}

CustomModel::~CustomModel()
{
}

void CustomModel::query()
{
	try{
		beginResetModel();
		db.item.query(opts, stat, items);
		endResetModel();
	} catch(...) {
		endResetModel();
		throw;
	}
}

void CustomModel::update(int row, Item &modified)
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

bool CustomModel::queryFilter(const Item &modified)
{
	bool want = false;
	if(modified.bought)
		want = true;
	if(modified.onStock && opts.stockOption == Query::StockOptions::ItemsOnStock)
		want = true;
	if(!modified.onStock && opts.stockOption == Query::StockOptions::ItemsUsedUp)
		want = true;
	return want;
}

void CustomModel::addShoppingItem(int row)
{
	const Item &i = item(row);
	Item shopItem;
	shopItem.uploaded = QDateTime::currentDateTime();
	shopItem.name = i.name;
	shopItem.category = i.category;
	db.item.insert(shopItem);
	itemChange(db, shopItem);
}
