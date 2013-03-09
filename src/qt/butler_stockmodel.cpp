/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_stockmodel.h"

StockModel::StockModel(Db & db) :
	ItemsModel(db)
{
	opts.stockOption = Query::ITEMS_ON_STOCK;
}

StockModel::~StockModel()
{
}

Qt::ItemFlags StockModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)items.size() && index.column() < Item::UnitPrice){
		if(index.column() == Item::OnStock)
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
		else
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

void StockModel::query()
{
	QueryStat stat;
	try{
		beginResetModel();
		db.item.query(opts, stat, items);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

void StockModel::addShoppingItem(int row)
{
	const Item &i = item(row);
	Item shopItem;
	shopItem.uploaded = QDateTime::currentDateTime();
	shopItem.name = i.name;
	shopItem.category = i.category;
	db.item.insert(shopItem);
	itemChange(shopItem);
}

void StockModel::drop(int row)
{
	Item &orig = items.queryAt(row);
	Item modified(orig);
	modified.onStock = false;
	db.item.update(orig, modified);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
	itemChange(modified);
}

void StockModel::update(int row, Item &modified)
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

bool StockModel::queryFilter(const Item &modified)
{
	return modified.bought && modified.onStock;
}
