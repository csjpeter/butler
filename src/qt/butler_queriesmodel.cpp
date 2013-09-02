/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_queriesmodel.h"

SCC TidQueryFieldName = QT_TRANSLATE_NOOP("QueriesModel", "Name");
SCC TidQueryFieldStartDate = QT_TRANSLATE_NOOP("QueriesModel", "From date");
SCC TidQueryFieldEndDate = QT_TRANSLATE_NOOP("QueriesModel", "Till date");

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
	if(index.row() < (int)queries.size() && index.column() < Query::NumOfFields){
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
			modified.name = value.toString();
			update(row, modified);
			break;
		case Query::StartDate :
			modified.startDate = value.toDateTime();
			update(row, modified);
			break;
		case Query::EndDate :
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

int QueriesModel::index(const QString & name) const
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
	db.query.del(query);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	queries.removeAt(row);
}

void QueriesModel::addNew(Query & query)
{
	db.query.insert(query);
	ModelInsertGuard g(this, QModelIndex(), queries.size(), queries.size());
	queries.add(new Query(query));
}

void QueriesModel::update(int row, Query & modified)
{
	Query & orig = queries.queryAt(row);
	db.query.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Query::NumOfFields-1));
}

void QueriesModel::query()
{
	ModelResetGuard g(this);
	db.query.query(queries);
}

QString QueriesModel::categoriesToString(const CategoryNameSet & cat)
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

QString QueriesModel::tagsToString(const TagNameSet & tags)
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

void QueriesModel::stringToCategories(const QString & value, CategoryNameSet & cat)
{
	cat.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		cat.add(new QString(sl.at(i).trimmed()));
}

void QueriesModel::stringToTags(const QString & value, TagNameSet & tags)
{
	tags.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		tags.add(new csjp::Text(sl.at(i).trimmed().utf16()));
}

void QueriesModel::sort(int column, bool ascending)
{
	if(queries.ascending == ascending && queries.ordering == column)
		return;

	ModelResetGuard g(this);
	queries.ascending = ascending;
	queries.ordering = static_cast<Query::Fields>(column);
	queries.sort();
}
