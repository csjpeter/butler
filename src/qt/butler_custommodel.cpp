/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_custommodel.h"

namespace Butler {

	CustomModel::CustomModel() :
		ItemsModel()
	{
	}

	CustomModel::~CustomModel()
	{
	}

	bool CustomModel::query()
	{
		beginResetModel();
		bool ret = db().item().query(opts, stat, items);
		endResetModel();
		return ret;
	}

	bool CustomModel::update(int row, Item &modified)
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

	bool CustomModel::queryFilter(const Item &modified)
	{
		bool want = false;
		if(modified.bought)
			want = true;
		if(modified.onStock && opts.stockOption == Query::ITEMS_ON_STOCK)
			want = true;
		if(!modified.onStock && opts.stockOption == Query::ITEMS_USED_UP)
			want = true;
		return want;
	}

}

