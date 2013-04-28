/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CUSTOMMODEL_H
#define BUTLER_CUSTOMMODEL_H

#include <butler_query_set.h>

#include "butler_itemsmodel.h"

class Item;

class CustomModel : public ItemsModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
	CustomModel(Db & db, const WaresModel & wmodel);
	virtual ~CustomModel();

	void query();
	virtual void update(int row, Item &modified);
	virtual bool queryFilter(const Item &modified);

	void addShoppingItem(int row);

public:
	QuerySet queries;
	Query opts;
	QueryStat stat;
};

#endif
