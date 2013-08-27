/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_tagsmodel.h"

SCC TidTagFieldName		= QT_TRANSLATE_NOOP("TagsModel", "Name");
SCC TidTagFieldDescription	= QT_TRANSLATE_NOOP("TagsModel", "Description");

TagsModel::TagsModel(Db & db) :
	db(db)
{
	query();
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
			index.column() < Tag::NumOfFields){
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

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
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
		case Tag::Name :
			tags.queryAt(index.row()).name = value.toString();
			break;
		case Tag::Description :
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

	return Tag::NumOfFields;
}

bool TagsModel::removeRows(
		int row, int count, const QModelIndex &parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool TagsModel::insertRows(
		int row, int count, const QModelIndex &parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void TagsModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
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
/*
void TagsModel::setTagSet(const TagSet &ts)
{
	ModelResetGuard g(this);
	tags.copy(ts);
}
*/

const Tag& TagsModel::tag(int row)
{
	return tags.queryAt(row);
}

void TagsModel::del(int row)
{
	Tag &tag = tags.queryAt(row);
	db.tag.del(tag);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	tags.removeAt(row);
}

void TagsModel::addNew(Tag &tag)
{
	db.tag.insert(tag);
	ModelInsertGuard g(this, QModelIndex(), tags.size(), tags.size());
	tags.add(new Tag(tag));
}

void TagsModel::update(int row, Tag &modified)
{
	Tag &orig = tags.queryAt(row);
	db.tag.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Tag::NumOfFields-1));
}

void TagsModel::query()
{
	ModelResetGuard g(this);
	db.tag.query(tags);
}

void TagsModel::sort(int column, bool ascending)
{
	if(tags.ascending == ascending && tags.ordering == column)
		return;

	ModelResetGuard g(this);
	tags.ascending = ascending;
	tags.ordering = static_cast<Tag::Fields>(column);
	tags.sort();
}
