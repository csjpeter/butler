/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <QtGui>

#include "butler_inventoriesmodel.h"

SCC TidInventoryFieldName = QT_TRANSLATE_NOOP("InventoriesModel", "Name");
SCC TidInventoryFieldComment = QT_TRANSLATE_NOOP("InventoriesModel", "Comment");

InventoriesModel::InventoriesModel(InventoryDb & db) :
	db(db)
{
	query();
}

InventoriesModel::~InventoriesModel()
{
}

QModelIndex InventoriesModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags InventoriesModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)inventories.size() && index.column() < Inventory::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant InventoriesModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)inventories.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case Inventory::Name :
			return QVariant(inventories.queryAt(index.row()).name);
			break;
		case Inventory::Comment :
			return QVariant(inventories.queryAt(index.row()).comment);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant InventoriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Inventory::Name :
			return QVariant(tr(TidInventoryFieldName));
			break;
		case Inventory::Comment :
			return QVariant(tr(TidInventoryFieldComment));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool InventoriesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)inventories.size() <= index.row())
		return false;

	int row = index.row();
	Inventory modified(inventories.queryAt(row));

	QString str;

	switch(index.column()){
		case Inventory::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Inventory::Comment :
			modified.comment <<= value;
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool InventoriesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int InventoriesModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return inventories.size();
}

int InventoriesModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Inventory::NumOfFields;
}

bool InventoriesModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool InventoriesModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void InventoriesModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int InventoriesModel::index(const Text & name) const
{
	if(inventories.has(name))
		return inventories.index(name);
	else
		return -1;
}

const Inventory& InventoriesModel::inventory(int row) const
{
	return inventories.queryAt(row);
}

void InventoriesModel::del(int row)
{
	Inventory & inventory = inventories.queryAt(row);
	db.del(inventory);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	inventories.removeAt(row);
}

void InventoriesModel::addNew(Inventory & inventory)
{
	db.insert(inventory);
	ModelInsertGuard g(this, QModelIndex(), inventories.size(), inventories.size());
	inventories.add(new Inventory(inventory));
}

void InventoriesModel::update(int row, Inventory & modified)
{
	Inventory & orig = inventories.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Inventory::NumOfFields-1));
}

void InventoriesModel::query()
{
	ModelResetGuard g(this);
	db.query(inventories);
}

void InventoriesModel::sort(int column, bool ascending)
{
	if(inventories.ascending == ascending && inventories.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	inventories.ascending = ascending;
    inventories.ordering.moveToFront(static_cast<Inventory::Fields>(column));
	inventories.sort();
}
