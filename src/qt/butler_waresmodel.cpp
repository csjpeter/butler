/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <QtGui>

#include "butler_waresmodel.h"

SCC TidWareFieldName = QT_TRANSLATE_NOOP("WaresModel", "Name");
SCC TidWareFieldUnit = QT_TRANSLATE_NOOP("WaresModel", "Unit");
SCC TidWareFieldTypes = QT_TRANSLATE_NOOP("WaresModel", "Types");
SCC TidWareFieldTags = QT_TRANSLATE_NOOP("WaresModel", "Tags");

WaresModel::WaresModel(WareDb & db) :
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
	if(index.row() < (int)wares.size() && index.column() < Ware::NumOfFields){
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
		case Ware::Name :
			return QVariant(wares.queryAt(index.row()).name);
			break;
		case Ware::Unit :
			return QVariant(wares.queryAt(index.row()).unit);
			break;
		case Ware::Types :
			return QVariant(typesToString(wares.queryAt(index.row()).types));
			break;
		case Ware::Tags :
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
		case Ware::Name :
			return QVariant(tr(TidWareFieldName));
			break;
		case Ware::Unit :
			return QVariant(tr(TidWareFieldUnit));
			break;
		case Ware::Types :
			return QVariant(tr(TidWareFieldTypes));
			break;
		case Ware::Tags :
			return QVariant(tr(TidWareFieldTags));
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
		case Ware::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Ware::Unit :
			modified.unit <<= value;
			update(row, modified);
			break;
		case Ware::Types :
			stringToTypes(value.toString(), modified.types);
			update(row, modified);
			break;
		case Ware::Tags :
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

	return Ware::NumOfFields;
}

bool WaresModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool WaresModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void WaresModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int WaresModel::index(const Text & name) const
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
	Ware & ware = wares.queryAt(row);
	db.del(ware);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	wares.removeAt(row);
}

void WaresModel::addNew(Ware & ware)
{
	db.insert(ware);
	ModelInsertGuard g(this, QModelIndex(), wares.size(), wares.size());
	wares.add(new Ware(ware));
}

void WaresModel::update(int row, Ware & modified)
{
	Ware & orig = wares.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Ware::NumOfFields-1));
}

void WaresModel::query()
{
	ModelResetGuard g(this);
	db.query(wares);
}

QString WaresModel::typesToString(const TypeNameSet & cat)
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

QString WaresModel::tagsToString(const TagNameSet & tags)
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

void WaresModel::stringToTypes(const QString & value, TypeNameSet & cat)
{
	cat.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		cat.add(new Text(sl.at(i).trimmed()));
}

void WaresModel::stringToTags(const QString & value, TagNameSet & tags)
{
	tags.clear();
	QStringList sl;
	sl = value.split(", ", QString::SkipEmptyParts);
	int s = sl.size();
	int i;
	for(i=0; i<s; i++)
		tags.add(new Text(sl.at(i).trimmed()));
}

void WaresModel::sort(int column, bool ascending)
{
	if(wares.ascending == ascending && wares.ordering == column)
		return;

	ModelResetGuard g(this);
	wares.ascending = ascending;
	wares.ordering = static_cast<Ware::Fields>(column);
	wares.sort();
}
