/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_custommodel.h"

CustomModel::CustomModel(ItemDb & db, const WaresModel & wmodel) :
	ItemsModel(db, wmodel)
{
}

CustomModel::~CustomModel()
{
}

void CustomModel::query()
{
	ModelResetGuard g(this);
	db.query(opts, stat, items);
}

void CustomModel::update(int row, Item & modified)
{
	ItemsModel::update(row, modified);
	if(queryFilter(modified))
		return;

	ModelRemoveGuard g(this, QModelIndex(), row, row);
	items.removeAt(row);
}

bool CustomModel::queryFilter(const Item & modified)
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
	const Item & i = item(row);
	Item shoppingItem;
	shoppingItem.uploaded <<= QDateTime::currentDateTime();
	shoppingItem.name = i.name;
	shoppingItem.category = i.category;
	db.insert(shoppingItem);
	itemChange(db, shoppingItem);
}
