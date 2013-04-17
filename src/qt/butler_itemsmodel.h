/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEMSMODEL_H
#define BUTLER_ITEMSMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include <csjp_reference_container.h>

#include <butler_item.h>
#include <butler_item_set.h>

#include <butler_db.h>

#include "butler_texts.h"
#include "butler_config.h"
#include "butler_waresmodel.h"

class Item;

class ItemsModel :
	public QAbstractTableModel
{
private:
	Q_OBJECT;

public:
	ItemsModel(Db & db, const WaresModel & wmodel);
	virtual ~ItemsModel();

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
			const QModelIndex & index, const QVariant & value,
			int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation,
			const QVariant & value, int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const QDateTime &uploaded) const NO_FCLOG;
	const Item & item(int row) const NO_FCLOG;
	const ItemSet & itemSet() const { return items; };
	void del(int row);
	void addNew(Item &item);
	virtual void update(int row, Item &modified);
	void sort(int logicalIndex, bool ascending);

protected:
	void itemChange(const Item &modified);
	void itemChangeListener(const Item &modified);
	/* return true if 'modified' should be in query list */
	virtual bool queryFilter(const Item &modified) = 0;
	void itemRemoved(const Item &removed);
	void itemRemovedListener(const Item &removed);

protected:
	Db & db;
	const WaresModel & wmodel;
	ItemSet items;

private:
	static csjp::ReferenceContainer<ItemsModel> itemOperationListeners;
};

bool operator<(const ItemsModel &a, const ItemsModel &b);

#endif
