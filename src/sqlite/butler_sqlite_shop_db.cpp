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

void ShopDb::create()
{
	bool ret;

	ret = shopTable.create();

	return ret;
}

void ShopDb::check(QStringList &tables)
{
	bool ret;

	ret = shopTable.check(tables);

	return ret;
}

void ShopDb::update()
{
	return true;
}

void ShopDb::insert(const Shop &s)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && shopTable.insert(s);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void ShopDb::update(const Shop &orig, const Shop &modified)
{
	bool ret = true;

	ret = sql.transaction();
	ret = ret && shopTable.update(orig, modified);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void ShopDb::del(const Shop &s)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && shopTable.del(s);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void ShopDb::query(ShopSet &ss)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && shopTable.query(ss);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

}

