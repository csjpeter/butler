/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shoppingmodel.h"
#include "butler_shopsmodel.h"

namespace Butler {

	ShoppingModel::ShoppingModel() :
		ItemsModel()
	{
	}

	ShoppingModel::~ShoppingModel()
	{
	}

	Qt::ItemFlags ShoppingModel::flags(const QModelIndex & index) const
	{
		if(index.row() < (int)items.size() && index.column() < Item::UnitPrice){
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		} else
			return Qt::NoItemFlags;
	}

	bool ShoppingModel::query()
	{
		beginResetModel();
		bool ret = db().item().query(queryTagNames, items);
		endResetModel();
		return ret;
	}

	bool ShoppingModel::buy(unsigned itemRow, Item &modified)
	{
		Item &orig = items.queryAt(itemRow);
		modified.bought = true;
		modified.onStock = true;
		if(db().item().update(orig, modified)){
			beginRemoveRows(QModelIndex(), itemRow, itemRow);
			items.removeAt(itemRow);
			endRemoveRows();
			itemChange(modified);
			return true;
		}
		return false;
	}

	bool ShoppingModel::update(int row, Item &modified)
	{
		if(ItemsModel::update(row, modified)){
			if(!queryFilter(modified)){
				beginRemoveRows(QModelIndex(), row, row);
				items.removeAt(row);
				endRemoveRows();
			}
			return true;
		}

		return false;
	}

	bool ShoppingModel::queryFilter(const Item &modified)
	{
		return !modified.bought;
	}

}
