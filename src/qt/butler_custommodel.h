/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CUSTOMMODEL_H
#define BUTLER_CUSTOMMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include <QModelIndex>

#include <butler_item.h>
#include <butler_item_set.h>
#include <butler_query.h>
#include <butler_query_set.h>

#include "butler_itemsmodel.h"

class Item;

class CustomModel : public ItemsModel
{
private:
	Q_OBJECT;

public:
	CustomModel();
	~CustomModel();

	void query();
	virtual bool update(int row, Item &modified);
	virtual bool queryFilter(const Item &modified);

public:
	QuerySet queries;
	Query opts;
	QueryStat stat;
};

#endif
