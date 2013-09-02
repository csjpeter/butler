/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_databasesmodel.h"
#include "butler_database_descriptor.h"

extern DatabaseDescriptorSet descriptorSet;

SCC TidQueryFieldName = QT_TRANSLATE_NOOP("DatabasesModel", "Name");
SCC TidQueryFieldDriver = QT_TRANSLATE_NOOP("DatabasesModel", "Driver");
SCC TidQueryFieldDatabaseName = QT_TRANSLATE_NOOP("DatabasesModel", "Database name");
SCC TidQueryFieldUsername = QT_TRANSLATE_NOOP("DatabasesModel", "Username");
SCC TidQueryFieldPassword = QT_TRANSLATE_NOOP("DatabasesModel", "Password");
SCC TidQueryFieldHost = QT_TRANSLATE_NOOP("DatabasesModel", "Host");
SCC TidQueryFieldPort = QT_TRANSLATE_NOOP("DatabasesModel", "Port");

DatabasesModel::DatabasesModel()
{
}

DatabasesModel::~DatabasesModel()
{
}

QModelIndex DatabasesModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags DatabasesModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)descriptorSet.size() && index.column() < DatabaseDescriptor::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant DatabasesModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)descriptorSet.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case DatabaseDescriptor::Name :
			return QVariant(descriptorSet.queryAt(index.row()).name);
			break;
		case DatabaseDescriptor::Driver :
			return QVariant(descriptorSet.queryAt(index.row()).driver);
			break;
		case DatabaseDescriptor::DatabaseName :
			return QVariant(descriptorSet.queryAt(index.row()).databaseName);
			break;
		case DatabaseDescriptor::Username :
			return QVariant(descriptorSet.queryAt(index.row()).username);
			break;
		case DatabaseDescriptor::Password :
			return QVariant(descriptorSet.queryAt(index.row()).password);
			break;
		case DatabaseDescriptor::Host :
			return QVariant(descriptorSet.queryAt(index.row()).host);
			break;
		case DatabaseDescriptor::Port :
			return QVariant(descriptorSet.queryAt(index.row()).port);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant DatabasesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case DatabaseDescriptor::Name :
			return QVariant(tr(TidQueryFieldName));
			break;
		case DatabaseDescriptor::Driver :
			return QVariant(tr(TidQueryFieldDriver));
			break;
		case DatabaseDescriptor::DatabaseName :
			return QVariant(tr(TidQueryFieldDatabaseName));
			break;
		case DatabaseDescriptor::Username :
			return QVariant(tr(TidQueryFieldUsername));
			break;
		case DatabaseDescriptor::Password :
			return QVariant(tr(TidQueryFieldPassword));
			break;
		case DatabaseDescriptor::Host :
			return QVariant(tr(TidQueryFieldHost));
			break;
		case DatabaseDescriptor::Port :
			return QVariant(tr(TidQueryFieldPort));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool DatabasesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)descriptorSet.size() <= index.row())
		return false;

	int row = index.row();
	DatabaseDescriptor & desc = descriptorSet.queryAt(row);

	QString str;

	switch(index.column()){
		case DatabaseDescriptor::Name :
			desc.name <<= value;
			break;
		case DatabaseDescriptor::Driver :
			desc.driver <<= value;
			break;
		case DatabaseDescriptor::DatabaseName :
			desc.databaseName <<= value;
			break;
		case DatabaseDescriptor::Username :
			desc.username <<= value;
			break;
		case DatabaseDescriptor::Password :
			desc.password <<= value;
			break;
		case DatabaseDescriptor::Host :
			desc.host <<= value;
			break;
		case DatabaseDescriptor::Port :
			desc.port <<= value;
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool DatabasesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int DatabasesModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return descriptorSet.size();
}

int DatabasesModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return DatabaseDescriptor::NumOfFields;
}

bool DatabasesModel::removeRows(int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	descriptorSet.removeAt(row);
	return true;
}

bool DatabasesModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void DatabasesModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int DatabasesModel::index(const QString & name) const
{
	if(descriptorSet.has(name))
		return descriptorSet.index(name);
	else
		return -1;
}

const DatabaseDescriptor & DatabasesModel::query(int row)
{
	return descriptorSet.queryAt(row);
}

void DatabasesModel::del(int row)
{
	removeRows(row, 1, QModelIndex());
}

void DatabasesModel::addNew(DatabaseDescriptor & desc)
{
	ModelInsertGuard g(this, QModelIndex(), descriptorSet.size(), descriptorSet.size());
	descriptorSet.add(new DatabaseDescriptor(desc));
}

void DatabasesModel::update(int row, DatabaseDescriptor & modified)
{
	descriptorSet.queryAt(row) = modified;
	dataChanged(index(row, 0), index(row, Query::NumOfFields-1));
}

bool DatabasesModel::has(const QString & str) const
{
	return descriptorSet.has(str);
}

void DatabasesModel::sort(int column, bool ascending)
{
	if(descriptorSet.ascending == ascending && descriptorSet.ordering == column)
		return;

	ModelResetGuard g(this);
	descriptorSet.ascending = ascending;
	descriptorSet.ordering = static_cast<DatabaseDescriptor::Fields>(column);
	descriptorSet.sort();
}
