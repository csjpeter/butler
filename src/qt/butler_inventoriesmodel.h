/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#if 0
#ifndef BUTLER_INVENTORIESMODEL_H
#define BUTLER_INVENTORIESMODEL_H

#include <QString>

#include <butler_abstract_table_model.h>

#include <butler_dbclasses.h>

#include <butler_config.h>

class InventoriesModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	InventoriesModel(InventoryDb & db);
	virtual ~InventoriesModel();

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
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Inventory& inventory(int row) const NO_FCLOG;
	const InventorySet & inventorySet() const { return inventories; };
	void del(int row);
	void addNew(Inventory & inventory);
	virtual void update(int row, Inventory & modified);
	void query();

	void sort(int column, bool ascending);

protected:
	InventoryDb & db;
	InventorySet inventories;
};

#endif
#endif
