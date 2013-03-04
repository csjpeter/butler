/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SHOP_DB_H
#define BUTLER_SQLITE_SHOP_DB_H

#include <butler_db.h>

#include "butler_sqlite_shop.h"

namespace Sqlite {

class ShopDb : public ::ShopDb
{
public:
	ShopDb(Sql &sql);
	virtual ~ShopDb();
private:
	ShopDb();

public:
	bool create();
	bool check(QStringList &tables);
	bool update();

	bool insert(const Shop &);
	bool update(const Shop &orig, const Shop &modified);
	bool del(const Shop &);
	bool query(ShopSet &);

private:
	Sql &sql;
	ShopTable shopTable;
};

}

#endif

