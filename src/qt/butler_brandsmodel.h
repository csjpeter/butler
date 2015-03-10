/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_BRANDSMODEL_H
#define BUTLER_BRANDSMODEL_H

#include <QString>

#include <butler_abstract_table_model.h>

#include <butler_dbclasses.h>

#include <butler_config.h>

class BrandsModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	BrandsModel(BrandDb & db);
	virtual ~BrandsModel();

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
	const Brand& brand(int row) const NO_FCLOG;
	const BrandSet & brandSet() const { return brands; };
	void del(int row);
	void addNew(Brand & brand);
	virtual void update(int row, Brand & modified);
	void query();

	void sort(int column, bool ascending);

protected:
	BrandDb & db;
	BrandSet brands;
};

#endif
