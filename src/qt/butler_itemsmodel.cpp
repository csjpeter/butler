/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include "butler_waresmodel.h"
#include "butler_itemsmodel.h"

#include <butler_databases.h> // FIXME cyclic dependency

csjp::ReferenceContainer<ItemsModel> ItemsModel::itemOperationListeners;

ItemsModel::ItemsModel(Db & db) :
	dbname(db.desc.name),
	db(db)
{
	itemOperationListeners.add(*this);
}

ItemsModel::~ItemsModel()
{
	itemOperationListeners.remove(*this);
}

QModelIndex ItemsModel::index(	int row,
		int column,
		const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)items.size() &&
			index.column() < Item::UnitPrice){
		if(index.column() != Item::Uploaded)
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
		else
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else if(index.row() < (int)items.size() &&
			index.column() < Item::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant ItemsModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)items.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Item::Name :
			return QVariant(items.queryAt(index.row()).name);
			break;
		case Item::Category :
			return QVariant(items.queryAt(index.row()).category);
			break;
		case Item::Uploaded :
			return QVariant(items.queryAt(index.row()).uploaded);
			break;
		case Item::Purchased :
			return QVariant(items.queryAt(index.row()).purchased);
			break;
		case Item::Shop :
			return QVariant(items.queryAt(index.row()).shop);
			break;
		case Item::Price :
			return QVariant(items.queryAt(index.row()).price);
			break;
		case Item::Quantity :
			{
				Item &item = items.queryAt(index.row());
				WaresModel &wm = waresModel(dbname);
				int i = wm.index(item.name);
				QString val;
				val.setNum(item.quantity, 'g', 3);
				if(i != -1) {
					const Ware &w = wm.ware(i);
					val += " ";
					val += w.unit;
				}
				return QVariant(val);
			}
			break;
		case Item::OnStock :
			return QVariant(items.queryAt(index.row()).onStock);
			break;
		case Item::Comment :
			return QVariant(items.queryAt(index.row()).comment);
			break;
		case Item::UnitPrice :
			{
				Item &item = items.queryAt(index.row());
				double price = 0;
				if(DBL_EPSILON <= item.quantity)
					price = item.price / item.quantity;
				return QVariant(price);
			}
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant ItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Item::Name :
			return QVariant("Common name");
			break;
		case Item::Category :
			return QVariant("Category name");
			break;
		case Item::Uploaded :
			return QVariant("Upload date");
			break;
		case Item::Purchased :
			return QVariant("Bought on");
			break;
		case Item::Shop :
			return QVariant("Shop");
			break;
		case Item::Price :
			return QVariant("Gross price");
			break;
		case Item::Quantity :
			return QVariant("Quantity");
			break;
		case Item::OnStock :
			return QVariant("On stock");
			break;
		case Item::Comment :
			return QVariant("Comment");
			break;
		case Item::UnitPrice :
			return QVariant("Unit price");
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)items.size() <= index.row())
		return false;

	int row = index.row();
	Item modified(items.queryAt(row));

	switch(index.column()){
		case Item::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case Item::Category :
			modified.category = value.toString();
			update(row, modified);
			break;
		case Item::Uploaded :
			modified.uploaded = value.toDateTime();
			update(row, modified);
			break;
		case Item::Purchased :
			modified.purchased = value.toDateTime();
			update(row, modified);
			break;
		case Item::Shop :
			modified.shop = value.toString();
			update(row, modified);
			break;
		case Item::Price :
			modified.price = value.toDouble();
			update(row, modified);
			break;
		case Item::Quantity :
			modified.quantity = value.toDouble();
			update(row, modified);
			break;
		case Item::OnStock :
			modified.onStock = value.toBool();
			update(row, modified);
			break;
		case Item::Comment :
			modified.comment = value.toString();
			update(row, modified);
			break;
		default :
			return false;
	}

	//		dataChanged(index, index);
	return true;
}

bool ItemsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemsModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return items.size();
}

int ItemsModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Item::NumOfFields;
}

bool ItemsModel::removeRows(
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

bool ItemsModel::insertRows(
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

int ItemsModel::index(const QDateTime &uploaded) const
{
	if(items.has(uploaded))
		return items.index(uploaded);
	else
		return -1;
}

const Item& ItemsModel::item(int row) const
{
	return items.queryAt(row);
}

void ItemsModel::del(int row)
{
	Item &item = items.queryAt(row);
	db.item.del(item);
	itemRemoved(item);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void ItemsModel::itemRemoved(const Item &removed)
{
	int s = itemOperationListeners.size();
	for(int i=0; i<s; i++){
		ItemsModel &model = itemOperationListeners.queryAt(i);
		if(&model == this)
			continue;
		model.itemRemovedListener(removed);
	}
}

void ItemsModel::itemRemovedListener(const Item &removed)
{
	if(!items.has(removed.uploaded))
		return;

	int row = items.index(removed.uploaded);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void ItemsModel::addNew(Item &item)
{
	db.item.insert(item);
	try {
		beginInsertRows(QModelIndex(), items.size(), items.size());
		items.add(new Item(item));
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
	itemChange(item);
}

void ItemsModel::update(int row, Item &modified)
{
	Item &orig = items.queryAt(row);

	db.item.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Item::NumOfFields-1));
	itemChange(modified);
}

void ItemsModel::itemChange(const Item &modified)
{
	int s = itemOperationListeners.size();
	for(int i=0; i<s; i++){
		ItemsModel &model = itemOperationListeners.queryAt(i);
		if(&model == this)
			continue;
		model.itemChangeListener(modified);
	}
}

void ItemsModel::itemChangeListener(const Item &modified)
{
	bool want = queryFilter(modified);
	if(items.has(modified.uploaded)){
		int row = items.index(modified.uploaded);
		if(want){
			items.queryAt(row) = modified;
			dataChanged(index(row, 0), index(row, Item::NumOfFields-1));
		} else {
			try {
				beginRemoveRows(QModelIndex(), row, row);
				items.removeAt(row);
				endRemoveRows();
			} catch (...) {
				endRemoveRows();
				throw;
			}
		}
	} else {
		if(want){
			try {
				beginInsertRows(QModelIndex(), items.size(), items.size());
				items.add(new Item(modified));
				endInsertRows();
			} catch (...) {
				endInsertRows();
				throw;
			}
		}
	}
}

void ItemsModel::sort(int column, bool ascending)
{
	if(items.ascending == ascending && items.ordering == column)
		return;

	try {
		beginResetModel();
		items.ascending = ascending;
		items.ordering = static_cast<Item::Fields>(column);
		items.sort();
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

bool operator<(const ItemsModel &a, const ItemsModel &b)
{
	return &a < &b;
}
