/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_custommodel.h"

CustomModel::CustomModel(Db & db, const WaresModel & wmodel) :
	ItemsModel(db, wmodel)
{
	query();
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
	if(modified.onStock && opts.stockOption == Query::ITEMS_ON_STOCK)
		want = true;
	if(!modified.onStock && opts.stockOption == Query::ITEMS_USED_UP)
		want = true;
	return want;
}
