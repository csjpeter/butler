/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_warecategoriesmodel.h"

SCC TidCategoryFieldName = QT_TRANSLATE_NOOP("WareCategoriesModel", "Brand/Type");

WareCategoriesModel::WareCategoriesModel(CategoryNameSet & cat) :
	categories(cat)
{
}

WareCategoriesModel::~WareCategoriesModel()
{
}

QModelIndex WareCategoriesModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags WareCategoriesModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)categories.size() && index.column() < 1){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant WareCategoriesModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)categories.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case 0 :
			return QVariant(categories.queryAt(index.row()));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant WareCategoriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case 0 :
			return QVariant(tr(TidCategoryFieldName));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool WareCategoriesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)categories.size() <= index.row())
		return false;

	int row = index.row();

	QString str;

	switch(index.column()){
		case 0 :
			categories.queryAt(row) <<= value;
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool WareCategoriesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int WareCategoriesModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return categories.size();
}

int WareCategoriesModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return 1;
}

bool WareCategoriesModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool WareCategoriesModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void WareCategoriesModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int WareCategoriesModel::index(const QString & name) const
{
	if(categories.has(name))
		return categories.index(name);
	else
		return -1;
}

const QString & WareCategoriesModel::category(int row) const
{
	return categories.queryAt(row);
}

void WareCategoriesModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	categories.removeAt(row);
}

void WareCategoriesModel::addNew(const QString & cat)
{
	ModelInsertGuard g(this, QModelIndex(), categories.size(), categories.size());
	categories.add(new Text(cat.utf16()));
}

void WareCategoriesModel::sort(int column, bool ascending)
{
	if(categories.ascending == ascending || column != 0)
		return;

	ModelResetGuard g(this);
	categories.ascending = ascending;
	categories.sort();
}
