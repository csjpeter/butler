/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGSMODEL_H
#define BUTLER_TAGSMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>

#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_tag.h>
#include <butler_tag_set.h>

#include "butler_localdb.h"

class QueryOptions;
class Tag;
class Item;

namespace Butler {

	class TagsModel :
		public QAbstractTableModel,
		public LocalDb
	{
		private:
			Q_OBJECT;

		public:
			enum Column {
				Name = 0,
				Description,
				NumOfColumns
			};

		public:
			static TagsModel& instance()
				__attribute__ ((no_instrument_function));
			~TagsModel();

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
			void setTagSet(const TagSet &);
			const TagSet& tagSet();
			const Tag& tag(int row)
				__attribute__ ((no_instrument_function));
			bool del(int row);
			bool addNew(Tag &tag);
			bool update(int row, Tag &modified);
			bool query();
			void sort(int column, bool ascending);

		protected:
			TagsModel();

		private:
			TagSet tags;
	};
}

#endif


