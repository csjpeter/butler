/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_waresmodel.h"

WaresModel::WaresModel(Db & db) :
	db(db)
{
	query();
}

WaresModel::~WaresModel()
{
}

QModelIndex WaresModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags WaresModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)wares.size() && index.column() < NumOfColumns){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant WaresModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)wares.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case WaresModel::Name :
			return QVariant(wares.queryAt(index.row()).name);
			break;
		case WaresModel::Unit :
			return QVariant(wares.queryAt(index.row()).unit);
			break;
		case WaresModel::Categories :
			return QVariant(categoriesToString(wares.queryAt(index.row()).categories));
			break;
		case WaresModel::Tags :
			return QVariant(tagsToString(wares.queryAt(index.row()).tags));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant WaresModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case WaresModel::Name :
			return QVariant("Name");
			break;
		case WaresModel::Unit :
			return QVariant("Unit");
			break;
		case WaresModel::Categories :
			return QVariant("Categories");
			break;
		case WaresModel::Tags :
			return QVariant("Tags");
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool WaresModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)wares.size() <= index.row())
		return false;

	int row = index.row();
	Ware modified(wares.queryAt(row));

	QString str;

	switch(index.column()){
		case WaresModel::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case WaresModel::Unit :
			modified.unit = value.toString();
			update(row, modified);
			break;
		case WaresModel::Categories :
			stringToCategories(value.toString(), modified.categories);
			update(row, modified);
			break;
		case WaresModel::Tags :
			stringToTags(value.toString(), modified.tags);
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool WaresModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int WaresModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return wares.size();
}

int WaresModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return NumOfColumns;
}

bool WaresModel::removeRows(
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

bool WaresModel::insertRows(
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

void WaresModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int WaresModel::index(const QString &name) const
{
	if(wares.has(name))
		return wares.index(name);
	else
		return -1;
}

const Ware& WaresModel::ware(int row) const
{
	return wares.queryAt(row);
}

void WaresModel::del(int row)
{
	Ware &ware = wares.queryAt(row);
	db.ware.del(ware);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		wares.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void WaresModel::addNew(Ware &ware)
{
	db.ware.insert(ware);
	try {
		beginInsertRows(QModelIndex(), wares.size(), wares.size());
		wares.add(new Ware(ware));
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
}

void WaresModel::update(int row, Ware &modified)
{
	Ware &orig = wares.queryAt(row);
	db.ware.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, WaresModel::NumOfColumns-1));
}

void WaresModel::query()
{
	try {
		beginResetModel();
		db.ware.query(wares);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

QString WaresModel::categoriesToString(const CategoryNameSet &cat)
{
	QString result("");

	int s = cat.size();
	if(s == 0)
		return result;

	result = cat.queryAt(0);
	int i;
	for(i=1; i<s; i++){
		result += ", ";
		result += cat.queryAt(i);
	}
	
	return result;
}

QString WaresModel::tagsToString(const TagNameSet &tags)
{
	QString result("");

	int s = tags.size();
	if(s == 0)
		return result;

	result = tags.queryAt(0);
	int i;
	for(i=1; i<s; i++){
		result += ", ";
		result += tags.queryAt(i);
	}
	
	return result;
}

void WaresModel::stringToCategories(const QString &value, CategoryNameSet &cat)
{
	cat.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		cat.add(new QString(sl.at(i).trimmed()));
}

void WaresModel::stringToTags(const QString &value, TagNameSet &tags)
{
	tags.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		tags.add(new QString(sl.at(i).trimmed()));
}

void WaresModel::sort(int column, bool ascending)
{
	if(wares.ascending == ascending && wares.ordering == column)
		return;

	try {
		beginResetModel();
		wares.ascending = ascending;
		wares.ordering = static_cast<Ware::Fields>(column);
		wares.sort();
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}
