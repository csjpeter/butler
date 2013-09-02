/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNERSMODEL_H
#define BUTLER_PARTNERSMODEL_H

#include <butler_abstract_table_model.h>

#include <butler_partner_db.h>

#include <butler_config.h>

class PartnersModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	PartnersModel(PartnerDb & db);
	virtual ~PartnersModel();

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
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const QString & name) const NO_FCLOG;
	const Partner& partner(int row) NO_FCLOG;
	const PartnerSet & partnerSet() const { return partners; };
	void del(int row);
	void addNew(Partner & partner);
	virtual void update(int row, Partner & modified);
	void query();
	void sort(int column, bool ascending);

protected:
	PartnerDb & db;
	PartnerSet partners;
};

#endif
