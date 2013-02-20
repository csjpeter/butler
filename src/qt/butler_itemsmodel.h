/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

#include "butler_config.h"
#include "butler_localdb.h"

class Item;

namespace Butler {

	class ItemsModel :
		public QAbstractTableModel,
		public LocalDb
	{
		private:
			Q_OBJECT;

		public:
			ItemsModel();
			virtual ~ItemsModel();

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
					const QModelIndex & parent = QModelIndex()) const;
			virtual bool removeRows(
					int row, int count, const QModelIndex &parent=QModelIndex());
			virtual bool insertRows(
					int row, int count, const QModelIndex &parent=QModelIndex());

		public:
			int index(const QDateTime &uploaded) const
				__attribute__ ((no_instrument_function));
			const Item & item(int row) const
				__attribute__ ((no_instrument_function));
			bool del(int row);
			bool addNew(Item &item);
			virtual bool update(int row, Item &modified);
			void sort(int logicalIndex, bool ascending);

		protected:
			void itemChange(const Item &modified);
			void itemChangeListener(const Item &modified);
			/* return true if 'modified' should be in query list */
			virtual bool queryFilter(const Item &modified) = 0;
			void itemRemoved(const Item &removed);
			void itemRemovedListener(const Item &removed);

		protected:
			ItemSet items;

		private:
			static csjp::ReferenceContainer<ItemsModel> itemOperationListeners;
	};

	bool operator<(const ItemsModel &a, const ItemsModel &b);
}

#endif

