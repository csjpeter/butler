/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shopsmodel.h"

ShopsModel::ShopsModel(Db & db) :
	db(db)
{
	query();
}

ShopsModel::~ShopsModel()
{
}

QModelIndex ShopsModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags ShopsModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)shops.size() && index.column() < Shop::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant ShopsModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)shops.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Shop::Name :
			return QVariant(shops.queryAt(index.row()).name);
			break;
		case Shop::StoreName :
			return QVariant(shops.queryAt(index.row()).storeName);
			break;
		case Shop::City :
			return QVariant(shops.queryAt(index.row()).city);
			break;
		case Shop::Address :
			return QVariant(shops.queryAt(index.row()).address);
			break;
		case Shop::Company :
			return QVariant(shops.queryAt(index.row()).company);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant ShopsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Shop::Name :
			return QVariant("Short name");
			break;
		case Shop::StoreName :
			return QVariant("Store name");
			break;
		case Shop::City :
			return QVariant("City");
			break;
		case Shop::Address :
			return QVariant("Address");
			break;
		case Shop::Company :
			return QVariant("Company");
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool ShopsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)shops.size() <= index.row())
		return false;

	int row = index.row();
	Shop modified(shops.queryAt(row));

	switch(index.column()){
		case Shop::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case Shop::StoreName :
			modified.storeName = value.toString();
			update(row, modified);
			break;
		case Shop::City :
			modified.city = value.toString();
			update(row, modified);
			break;
		case Shop::Address :
			modified.address = value.toString();
			update(row, modified);
			break;
		case Shop::Company :
			modified.company = value.toString();
			update(row, modified);
			break;
		default :
			return false;
	}

	//		dataChanged(index, index);
	return true;
}

bool ShopsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ShopsModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return shops.size();
}

int ShopsModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Shop::NumOfFields;
}

bool ShopsModel::removeRows(
		int row, int count, const QModelIndex &parent)
{
	try {
		beginRemoveRows(parent, row, row + count - 1);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
	return true;
}

bool ShopsModel::insertRows(
		int row, int count, const QModelIndex &parent)
{
	try {
		beginInsertRows(parent, row, row + count - 1);
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
	return true;
}

int ShopsModel::index(const QString &name) const
{
	if(shops.has(name))
		return shops.index(name);
	else
		return -1;
}

const Shop& ShopsModel::shop(int row)
{
	return shops.queryAt(row);
}

void ShopsModel::del(int row)
{
	Shop &shop = shops.queryAt(row);
	db.shop.del(shop);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		shops.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void ShopsModel::addNew(Shop &shop)
{
	db.shop.insert(shop);
	try {
		beginInsertRows(QModelIndex(), shops.size(), shops.size());
		shops.add(new Shop(shop));
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
}

void ShopsModel::update(int row, Shop &modified)
{
	Shop &orig = shops.queryAt(row);
	db.shop.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Shop::NumOfFields-1));
}

void ShopsModel::query()
{
	try {
		beginResetModel();
		db.shop.query(shops);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

void ShopsModel::sort(int column, bool ascending)
{
	if(shops.ascending == ascending && shops.ordering == column)
		return;

	try {
		beginResetModel();
		shops.ascending = ascending;
		shops.ordering = static_cast<Shop::Fields>(column);
		shops.sort();
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}
