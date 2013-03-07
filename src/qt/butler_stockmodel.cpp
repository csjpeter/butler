/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_stockmodel.h"

StockModel::StockModel() :
	ItemsModel()
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

bool StockModel::query()
{
	QueryStat stat;
	beginResetModel();
	bool ret = db.item.query(opts, stat, items);
	endResetModel();
	return ret;
}

bool StockModel::addShoppingItem(int row)
{
	const Item &i = item(row);
	Item shopItem;
	shopItem.uploaded = QDateTime::currentDateTime();
	shopItem.name = i.name;
	shopItem.category = i.category;
	bool ret = db.item.insert(shopItem);
	if(ret)
		itemChange(shopItem);
	return ret;
}

bool StockModel::drop(int row)
{
	Item &orig = items.queryAt(row);
	Item modified(orig);
	modified.onStock = false;
	if(db.item.update(orig, modified)){
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAt(row);
		endRemoveRows();
		itemChange(modified);
		return true;
	}
	return false;
}

bool StockModel::update(int row, Item &modified)
{
	if(ItemsModel::update(row, modified)){
		if(!queryFilter(modified)){
			beginRemoveRows(QModelIndex(), row, row);
			items.removeAt(row);
			endRemoveRows();
		}
		return true;
	}

	return false;
}

bool StockModel::queryFilter(const Item &modified)
{
	return modified.bought && modified.onStock;
}
