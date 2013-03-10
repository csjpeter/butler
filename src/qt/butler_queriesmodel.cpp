/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_queriesmodel.h"

QueriesModel::QueriesModel(Db & db) :
	db(db)
{
	query();
}

QueriesModel::~QueriesModel()
{
}

QModelIndex QueriesModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags QueriesModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)queries.size() && index.column() < NumOfColumns){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant QueriesModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)queries.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case QueriesModel::Name :
			return QVariant(queries.queryAt(index.row()).name);
			break;
		case QueriesModel::StartDate :
			return QVariant(queries.queryAt(index.row()).startDate);
			break;
		case QueriesModel::EndDate :
			return QVariant(queries.queryAt(index.row()).endDate);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant QueriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case QueriesModel::Name :
			return QVariant("Name");
			break;
		case QueriesModel::StartDate :
			return QVariant("Start date");
			break;
		case QueriesModel::EndDate :
			return QVariant("End date");
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool QueriesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)queries.size() <= index.row())
		return false;

	int row = index.row();
	Query modified(queries.queryAt(row));

	QString str;

	switch(index.column()){
		case QueriesModel::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case QueriesModel::StartDate :
			modified.startDate = value.toDateTime();
			update(row, modified);
			break;
		case QueriesModel::EndDate :
			modified.endDate = value.toDateTime();
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool QueriesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int QueriesModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return queries.size();
}

int QueriesModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return NumOfColumns;
}

bool QueriesModel::removeRows(
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

bool QueriesModel::insertRows(
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

int QueriesModel::index(const QString &name) const
{
	if(queries.has(name))
		return queries.index(name);
	else
		return -1;
}

const Query& QueriesModel::query(int row)
{
	return queries.queryAt(row);
}

void QueriesModel::del(int row)
{
	Query &query = queries.queryAt(row);
	db.query.del(query);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		queries.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void QueriesModel::addNew(Query &query)
{
	db.query.insert(query);
	try {
		beginInsertRows(QModelIndex(), queries.size(), queries.size());
		queries.add(new Query(query));
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
}

void QueriesModel::update(int row, Query &modified)
{
	Query &orig = queries.queryAt(row);
	db.query.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, QueriesModel::NumOfColumns-1));
}

void QueriesModel::query()
{
	try {
		beginResetModel();
		db.query.query(queries);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

QString QueriesModel::categoriesToString(const CategoryNameSet &cat)
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

QString QueriesModel::tagsToString(const TagNameSet &tags)
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

void QueriesModel::stringToCategories(const QString &value, CategoryNameSet &cat)
{
	cat.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		cat.add(new QString(sl.at(i).trimmed()));
}

void QueriesModel::stringToTags(const QString &value, TagNameSet &tags)
{
	tags.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		tags.add(new QString(sl.at(i).trimmed()));
}

void QueriesModel::sort(int column, bool ascending)
{
	if(queries.ascending == ascending && queries.ordering == column)
		return;

	try {
		beginResetModel();
		queries.ascending = ascending;
		queries.ordering = static_cast<Query::Fields>(column);
		queries.sort();
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}
