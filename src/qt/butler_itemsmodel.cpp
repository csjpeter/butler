/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include "butler_itemsmodel.h"

csjp::ReferenceContainer<ItemsModel> ItemsModel::itemOperationListeners;

ItemsModel::ItemsModel(Db & db, const WaresModel & wmodel) :
	db(db),
	wmodel(wmodel)
{
	itemOperationListeners.add(*this);
}

ItemsModel::~ItemsModel()
{
	/* Since ItemsModel instances are held in a static storage class container somewhere,
	 * it might happen on destruction time, that itemOperationListener (which is also a
	 * static storage class object) got destroyed before some Itemsmodel instances. */
	if(itemOperationListeners.has(*this))
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
		case Item::Bought :
			return QVariant(true);
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
				int i = wmodel.index(item.name);
				QString val;
				val.setNum(item.quantity, 'g', 3);
				if(i != -1) {
					const Ware &w = wmodel.ware(i);
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
			return QVariant(items.queryAt(index.row()).unitPrice());
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
		return QVariant("");

	switch(section){
		case Item::Name :
			return QVariant(qtTrId(TidItemFieldCommonName));
			break;
		case Item::Category :
			return QVariant(qtTrId(TidItemFieldCategory));
			break;
		case Item::Uploaded :
			return QVariant(qtTrId(TidItemFieldUploadDate));
			break;
		case Item::Bought :
			return QVariant(qtTrId(TidItemFieldIsBought));
			break;
		case Item::Purchased :
			return QVariant(qtTrId(TidItemFieldPurchaseDate));
			break;
		case Item::Shop :
			return QVariant(qtTrId(TidItemFieldShop));
			break;
		case Item::Price :
			return QVariant(qtTrId(TidItemFieldGrossPrice));
			break;
		case Item::Quantity :
			return QVariant(qtTrId(TidItemFieldQuantity));
			break;
		case Item::OnStock :
			return QVariant(qtTrId(TidItemFieldOnStock));
			break;
		case Item::Comment :
			return QVariant(qtTrId(TidItemFieldComment));
			break;
		case Item::UnitPrice :
			return QVariant(qtTrId(TidItemFieldUnitPrice));
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

	//dataChanged(index, index);
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

void ItemsModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
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
	itemRemoved(db, item);
}

void ItemsModel::itemRemoved(const Db & db, const Item &removed)
{
	int s = itemOperationListeners.size();
	for(int i=0; i<s; i++)
		itemOperationListeners.queryAt(i).itemRemovedListener(db, removed);
}

void ItemsModel::itemRemovedListener(const Db & db, const Item &removed)
{
	if(&db != &(this->db))
		return;

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
	itemChange(db, item);
}

void ItemsModel::update(int row, Item &modified)
{
	Item &orig = items.queryAt(row);

	db.item.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Item::NumOfFields-1));
	itemChange(db, modified);
}

void ItemsModel::itemChange(const Db & db, const Item &modified)
{
	int s = itemOperationListeners.size();
	for(int i=0; i<s; i++)
		itemOperationListeners.queryAt(i).itemChangeListener(db, modified);
}

void ItemsModel::itemChangeListener(const Db & db, const Item &modified)
{
	if(&db != &(this->db))
		return;

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
