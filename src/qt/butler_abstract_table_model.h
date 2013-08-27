/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_ABSTRACT_TABLE_MODEL_H
#define BUTLER_ABSTRACT_TABLE_MODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

class AbstractTableModel : public QAbstractTableModel
{
public:
		class ModelResetGuard
		{
		public:
				ModelResetGuard(AbstractTableModel * m) : model(*m)
				{
						model.beginResetModel();
				}

				~ModelResetGuard()
				{
						model.endResetModel();
				}
		private:
				AbstractTableModel & model;
		};

		class ModelRemoveGuard
		{
		public:
				ModelRemoveGuard(AbstractTableModel * m, const QModelIndex & parent, int first, int last) : model(*m)
				{
						model.beginRemoveRows(parent, first, last);
				}

				~ModelRemoveGuard()
				{
						model.endRemoveRows();
				}
		private:
				AbstractTableModel & model;
		};

		class ModelInsertGuard
		{
		public:
				ModelInsertGuard(AbstractTableModel * m, const QModelIndex & parent, int first, int last) : model(*m)
				{
						model.beginInsertRows(parent, first, last);
				}

				~ModelInsertGuard()
				{
						model.endInsertRows();
				}
		private:
				AbstractTableModel & model;
		};

};

#endif
