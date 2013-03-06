/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_sqlite_sql.h"
#include "butler_sqlite_shop_db.h"

namespace Sqlite {

ShopDb::ShopDb(Sql &_sql) :
	sql(_sql),
	shopTable(_sql)
{
}

ShopDb::~ShopDb()
{
}

void ShopDb::check(QStringList &tables)
{
	shopTable.check(tables);
}

void ShopDb::insert(const Shop &s)
{
	sql.transaction();
	shopTable.insert(s);
	sql.commit();
}

void ShopDb::update(const Shop &orig, const Shop &modified)
{
	sql.transaction();
	shopTable.update(orig, modified);
	sql.commit();
}

void ShopDb::del(const Shop &s)
{
	sql.transaction();
	shopTable.del(s);
	sql.commit();
}

void ShopDb::query(ShopSet &ss)
{
	sql.transaction();
	shopTable.query(ss);
	sql.commit();
}

}

