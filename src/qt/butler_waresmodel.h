/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARESMODEL_H
#define BUTLER_WARESMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include <butler_ware_set.h>

#include "butler_config.h"
#include "butler_localdb.h"

class WaresModel :
	public QAbstractTableModel,
	public LocalDb
{
private:
	Q_OBJECT;

public:
	enum Column {
		Name = 0,
		Unit,
		Categories,
		Tags,
		NumOfColumns
	};

public:
	static WaresModel& instance()
		__attribute__ ((no_instrument_function));
	~WaresModel();

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

public:
	int index(const QString &name) const
		__attribute__ ((no_instrument_function));
	const Ware& ware(int row)
		__attribute__ ((no_instrument_function));
	bool del(int row);
	bool addNew(Ware &ware);
	virtual bool update(int row, Ware &modified);
	bool query();

	static QString categoriesToString(const CategoryNameSet &cat)
		__attribute__ ((no_instrument_function));
	static QString tagsToString(const TagNameSet &tags)
		__attribute__ ((no_instrument_function));
	static void stringToCategories(const QString &value, CategoryNameSet &modified);
	static void stringToTags(const QString &value, TagNameSet &modified);
	void sort(int column, bool ascending);

protected:
	WaresModel();

protected:
	WareSet wares;
};

#endif
