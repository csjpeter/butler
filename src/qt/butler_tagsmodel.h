/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGSMODEL_H
#define BUTLER_TAGSMODEL_H

#include <butler_abstract_table_model.h>

#include <butler_tag_db.h>

#include <butler_config.h>

class QueryOptions;
class Tag;
class Item;

class TagsModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	TagsModel(TagDb & db);
	virtual ~TagsModel();

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
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const csjp::Text & name) const NO_FCLOG;
	//void setTagSet(const TagSet &);
	const TagSet& tagSet() const { return tags; }
	bool has(const QString & name) const { return tags.has(csjp::Text(name.utf16())); }
	const Tag& tag(int row) NO_FCLOG;
	void del(int row);
	void addNew(Tag & tag);
	void update(int row, Tag & modified);
	void query();
	void sort(int column, bool ascending);

private:
	TagDb & db;
	TagSet tags;
};

#endif
