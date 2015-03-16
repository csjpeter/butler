/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */
#if 0
#include <QtGui>

#include "butler_queriesmodel.h"

SCC TidQueryFieldName = QT_TRANSLATE_NOOP("QueriesModel", "Name");
SCC TidQueryFieldStartDate = QT_TRANSLATE_NOOP("QueriesModel", "From date");
SCC TidQueryFieldEndDate = QT_TRANSLATE_NOOP("QueriesModel", "Till date");

QueriesModel::QueriesModel(QueryDb & db) :
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
	if(index.row() < (int)queries.size() && index.column() < Query::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant QueriesModel::data(const QModelIndex & index, int role) const 
{
	QVariant value;

	if(!index.isValid())
		return value;

	if(index.parent().isValid())
		return value;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return value;

	if((int)queries.size() <= index.row())
		return value;
	
	QString result;

	switch(index.column()){
		case Query::Name :
			return QVariant(queries.queryAt(index.row()).name);
			break;
		case Query::StartDate :
			return QVariant(queries.queryAt(index.row()).startDate);
			break;
		case Query::EndDate :
			return QVariant(queries.queryAt(index.row()).endDate);
			break;
		default :
			return value;
	}

	return value;
}

QVariant QueriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Query::Name :
			return QVariant(tr(TidQueryFieldName));
			break;
		case Query::StartDate :
			return QVariant(tr(TidQueryFieldStartDate));
			break;
		case Query::EndDate :
			return QVariant(tr(TidQueryFieldEndDate));
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
		case Query::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Query::StartDate :
			modified.startDate <<= value;
			update(row, modified);
			break;
		case Query::EndDate :
			modified.endDate <<= value;
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

	return Query::NumOfFields;
}

bool QueriesModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool QueriesModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void QueriesModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int QueriesModel::index(const Text & name) const
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
	Query & query = queries.queryAt(row);
	db.del(query);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	queries.removeAt(row);
}

void QueriesModel::addNew(Query & query)
{
	db.insert(query);
	ModelInsertGuard g(this, QModelIndex(), queries.size(), queries.size());
	queries.add(new Query(query));
}

void QueriesModel::update(int row, Query & modified)
{
	Query & orig = queries.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Query::NumOfFields-1));
}

void QueriesModel::query()
{
	ModelResetGuard g(this);
	db.query(queries);
}

void QueriesModel::sort(int column, bool ascending)
{
	if(queries.ascending == ascending && queries.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	queries.ascending = ascending;
	queries.ordering.moveToFront(static_cast<Query::Fields>(column));
	queries.sort();
}
#endif
