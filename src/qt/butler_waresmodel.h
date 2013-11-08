/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARESMODEL_H
#define BUTLER_WARESMODEL_H

#include <QString>

#include <butler_abstract_table_model.h>

#include <butler_ware_db.h>

#include <butler_config.h>

class WaresModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	WaresModel(WareDb & db);
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
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Ware& ware(int row) const NO_FCLOG;
	const WareSet & wareSet() const { return wares; };
	void del(int row);
	void addNew(Ware & ware);
	virtual void update(int row, Ware & modified);
	void query();

	static QString typesToString(const TypeNameSet & cat) NO_FCLOG;
	static QString tagsToString(const TagNameSet & tags) NO_FCLOG;
	static void stringToTypes(const QString & value, TypeNameSet & modified);
	static void stringToTags(const QString & value, TagNameSet & modified);
	void sort(int column, bool ascending);

protected:
	WareDb & db;
	WareSet wares;
};

#endif
