/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASESMODEL_H
#define BUTLER_DATABASESMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include <butler_databases.h>

class DatabasesModel :
	public QAbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	DatabasesModel();
	virtual ~DatabasesModel();

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
	virtual bool setData(const QModelIndex & index, const QVariant & value,
			int role = Qt::EditRole);
	virtual bool setHeaderData(int section, Qt::Orientation orientation,
			const QVariant & value, int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const QString &name) const NO_FCLOG;
	const DatabaseDescriptor & query(int row) NO_FCLOG;
	void del(int row);
	void addNew(DatabaseDescriptor & desc);
	virtual void update(int row, DatabaseDescriptor & modified);
	bool has(const QString & str) const;

	void sort(int column, bool ascending);

protected:
};

#endif
