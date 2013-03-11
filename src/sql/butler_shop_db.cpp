/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_shop_db.h"

ShopDb::ShopDb(SqlConnection &_sql) :
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
	try {
		shopTable.insert(s);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ShopDb::update(const Shop &orig, const Shop &modified)
{
	sql.transaction();
	try {
		shopTable.update(orig, modified);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ShopDb::del(const Shop &s)
{
	sql.transaction();
	try {
		shopTable.del(s);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ShopDb::query(ShopSet &ss)
{
	sql.transaction();
	try {
		shopTable.query(ss);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}
