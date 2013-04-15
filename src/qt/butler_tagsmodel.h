/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGSMODEL_H
#define BUTLER_TAGSMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>

#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_tag.h>
#include <butler_tag_set.h>

#include <butler_db.h>

class QueryOptions;
class Tag;
class Item;

class TagsModel :
	public QAbstractTableModel
{
private:
	Q_OBJECT;

public:
	TagsModel(Db & db);
	virtual ~TagsModel();

	virtual QModelIndex index(
			int row,
			int column,
			const QModelIndex & parent = QModelIndex()) const
		__attribute__ ((no_instrument_function));
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const
		__attribute__ ((no_instrument_function));
	virtual QVariant data(
			const QModelIndex & index,
			int role = Qt::DisplayRole) const
		__attribute__ ((no_instrument_function));
	virtual QVariant headerData(
			int section,
			Qt::Orientation orientation,
			int role = Qt::DisplayRole) const
		__attribute__ ((no_instrument_function));
	virtual bool setData(
			const QModelIndex & index,
			const QVariant & value,
			int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section,
			Qt::Orientation orientation,
			const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(
			const QModelIndex & parent = QModelIndex()) const
		__attribute__ ((no_instrument_function));
	virtual int columnCount(
			const QModelIndex & parent = QModelIndex()) const
		__attribute__ ((no_instrument_function));
	virtual bool removeRows(
			int row, int count, const QModelIndex &parent=QModelIndex());
	virtual bool insertRows(
			int row, int count, const QModelIndex &parent=QModelIndex());
public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const QString &name) const
		__attribute__ ((no_instrument_function));
	//void setTagSet(const TagSet &);
	const TagSet& tagSet();
	const Tag& tag(int row)
		__attribute__ ((no_instrument_function));
	void del(int row);
	void addNew(Tag &tag);
	void update(int row, Tag &modified);
	void query();
	void sort(int column, bool ascending);

private:
	Db & db;
	TagSet tags;
};

#endif
