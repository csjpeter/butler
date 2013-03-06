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

	void CustomModel::query()
	{
		try{
			beginResetModel();
			db().item().query(opts, stat, items);
			endResetModel();
		} catch(...) {
			endResetModel();
			throw;
		}
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

