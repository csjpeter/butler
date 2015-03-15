/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */
#if 0
#include <QtGui>

#include "butler_tagsmodel.h"

SCC TidTagFieldName		= QT_TRANSLATE_NOOP("TagModel", "Name");
SCC TidTagFieldDescription	= QT_TRANSLATE_NOOP("TagModel", "Description");

TagModel::TagModel(TagDb & db) :
	db(db)
{
	query();
}

TagModel::~TagModel()
{
}

QModelIndex TagModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags TagModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)tags.size() && 
			index.column() < Tag::NumOfFields){
		if(index.column() != 8)
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
		else
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant TagModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)tags.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Tag::Name :
			return tags.queryAt(index.row()).name;
			break;
		case Tag::Description :
			return tags.queryAt(index.row()).description;
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant TagModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Tag::Name :
			return QVariant(tr(TidTagFieldName));
			break;
		case Tag::Description :
			return QVariant(tr(TidTagFieldDescription));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool TagModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)tags.size() <= index.row())
		return false;

	switch(index.column()){
		case Tag::Name :
			tags.queryAt(index.row()).name <<= value;
			break;
		case Tag::Description :
			tags.queryAt(index.row()).description <<= value;
			break;
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool TagModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int TagModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return tags.size();
}

int TagModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Tag::NumOfFields;
}

bool TagModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool TagModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void TagModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(tags.ascending == ascending && tags.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	tags.ascending = ascending;
	tags.ordering.moveToFront(static_cast<Tag::Fields>(column));
	tags.sort();
}

int TagModel::index(const Text & name) const
{
	if(tags.has(name))
		return tags.index(name);
	else
		return -1;
}

const Tag& TagModel::tag(int row)
{
	return tags.queryAt(row);
}

void TagModel::del(int row)
{
	Tag & tag = tags.queryAt(row);
	db.del(tag);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	tags.removeAt(row);
}

void TagModel::addNew(Tag & tag)
{
	db.insert(tag);
	ModelInsertGuard g(this, QModelIndex(), tags.size(), tags.size());
	tags.add(new Tag(tag));
}

void TagModel::update(int row, Tag & modified)
{
	Tag & orig = tags.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Tag::NumOfFields-1));
}

void TagModel::query()
{
	ModelResetGuard g(this);
	db.query(tags);
}
#endif
