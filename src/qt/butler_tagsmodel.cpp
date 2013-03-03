/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_tagsmodel.h"

namespace Butler {

TagsModel& TagsModel::instance()
{
	static TagsModel _instance;
	return _instance;
}

TagsModel::TagsModel() :
	LocalDb()
	{
	}

TagsModel::~TagsModel()
{
}

QModelIndex TagsModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags TagsModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)tags.size() && 
			index.column() < NumOfColumns){
		if(index.column() != 8)
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
		else
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant TagsModel::data(const QModelIndex & index, int role) const 
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
		case TagsModel::Name :
			return tags.queryAt(index.row()).name;
			break;
		case TagsModel::Description :
			return tags.queryAt(index.row()).description;
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case TagsModel::Name :
			return QVariant("Tag name");
			break;
		case TagsModel::Description :
			return QVariant("Description");
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool TagsModel::setData(const QModelIndex & index, const QVariant & value, int role)
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
		case TagsModel::Name :
			tags.queryAt(index.row()).name = value.toString();
			break;
		case TagsModel::Description :
			tags.queryAt(index.row()).description = value.toString();
			break;
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool TagsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int TagsModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return tags.size();
}

int TagsModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return NumOfColumns;
}

bool TagsModel::removeRows(
		int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	endRemoveRows();
	return true;
}

bool TagsModel::insertRows(
		int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count - 1);
	endInsertRows();
	return true;
}

int TagsModel::index(const QString &name) const
{
	if(tags.has(name))
		return tags.index(name);
	else
		return -1;
}

/* FIXME find better (reference) solution to avoid full copy and thus
 * have changes automatically adapted by other models and view. */
void TagsModel::setTagSet(const TagSet &ts)
{
	beginResetModel();
	tags.copy(ts);
	endResetModel();
}

const TagSet& TagsModel::tagSet()
{
	return TagsModel::tags;
}

const Tag& TagsModel::tag(int row)
{
	return tags.queryAt(row);
}

bool TagsModel::del(int row)
{
	Tag &tag = tags.queryAt(row);
	bool ret = false;
	if(db().tag().del(tag)){
		beginRemoveRows(QModelIndex(), row, row);
		tags.removeAt(row);
		endRemoveRows();
		ret = true;
	}
	return ret;
}

bool TagsModel::addNew(Tag &tag)
{
	bool ret = false;
	if(db().tag().insert(tag)){
		beginInsertRows(QModelIndex(), tags.size(), tags.size());
		tags.add(new Tag(tag));
		endInsertRows();
		ret = true;
	}
	return ret;
}

bool TagsModel::update(int row, Tag &modified)
{
	Tag &orig = tags.queryAt(row);

	if(db().tag().update(orig, modified)){
		orig = modified;
		dataChanged(index(row, 0), index(row, TagsModel::NumOfColumns-1));
		return true;
	}
	return false;
}

bool TagsModel::query()
{
	beginResetModel();
	bool ret = db().tag().query(tags);
	endResetModel();
	return ret;
}

void TagsModel::sort(int column, bool ascending)
{
	if(tags.ascending == ascending && tags.ordering == column)
		return;

	beginResetModel();
	tags.ascending = ascending;
	tags.ordering = static_cast<Tag::Fields>(column);
	tags.sort();
	endResetModel();
}

}

