/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SHOP_DB_H
#define BUTLER_SQLITE_SHOP_DB_H

#include <butler_db.h>

#include "butler_sqlite_shop.h"

class ShopDb : public ::ShopDb
{
public:
	ShopDb(SqlConnection &sql);
	virtual ~ShopDb();
private:
	ShopDb();

public:
	void check(QStringList &tables);

	void insert(const Shop &);
	void update(const Shop &orig, const Shop &modified);
	void del(const Shop &);
	void query(ShopSet &);

private:
	SqlConnection &sql;
	ShopTable shopTable;
};

#endif
