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
	void create();
	void check(QStringList &tables);
	void update();

	void insert(const Shop &);
	void update(const Shop &orig, const Shop &modified);
	void del(const Shop &);
	void query(ShopSet &);

private:
	Sql &sql;
	ShopTable shopTable;
};

}

#endif

