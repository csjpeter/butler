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

#include <butler_db.h>

#include <butler_config.h>

class WaresModel :
	public QAbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	WaresModel(Db & db);
	virtual ~WaresModel();

	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation,
			const QVariant & value, int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const QString &name) const NO_FCLOG;
	const Ware& ware(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Ware &ware);
	virtual void update(int row, Ware &modified);
	void query();

	static QString categoriesToString(const CategoryNameSet &cat) NO_FCLOG;
	static QString tagsToString(const TagNameSet &tags) NO_FCLOG;
	static void stringToCategories(const QString &value, CategoryNameSet &modified);
	static void stringToTags(const QString &value, TagNameSet &modified);
	void sort(int column, bool ascending);

protected:
	Db & db;
	WareSet wares;
};

#endif
