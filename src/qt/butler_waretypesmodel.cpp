/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_waretypesmodel.h"

SCC TidTypeFieldName = QT_TRANSLATE_NOOP("WareTypesModel", "Brand/Type");

WareTypesModel::WareTypesModel(TypeNameSet & cat) :
	types(cat)
{
}

WareTypesModel::~WareTypesModel()
{
}

QModelIndex WareTypesModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags WareTypesModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)types.size() && index.column() < 1){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant WareTypesModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)types.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case 0 :
			return QVariant(types.queryAt(index.row()));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant WareTypesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case 0 :
			return QVariant(tr(TidTypeFieldName));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool WareTypesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)types.size() <= index.row())
		return false;

	int row = index.row();

	QString str;

	switch(index.column()){
		case 0 :
			types.queryAt(row) <<= value;
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool WareTypesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int WareTypesModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return types.size();
}

int WareTypesModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return 1;
}

bool WareTypesModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool WareTypesModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void WareTypesModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int WareTypesModel::index(const QString & name) const
{
	if(types.has(name))
		return types.index(name);
	else
		return -1;
}

const QString & WareTypesModel::type(int row) const
{
	return types.queryAt(row);
}

void WareTypesModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	types.removeAt(row);
}

void WareTypesModel::addNew(const QString & cat)
{
	ModelInsertGuard g(this, QModelIndex(), types.size(), types.size());
	types.add(new Text(cat));
}

void WareTypesModel::sort(int column, bool ascending)
{
	if(types.ascending == ascending || column != 0)
		return;

	ModelResetGuard g(this);
	types.ascending = ascending;
	types.sort();
}
