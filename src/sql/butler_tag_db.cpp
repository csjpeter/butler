/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_tag_db.h"

TagDb::TagDb(SqlConnection &_sql) :
	sql(_sql),
	tagTable(_sql)
{
	DBG("sql: %p", &sql);
}

TagDb::~TagDb()
{
}

void TagDb::check(QStringList &tables)
{
	tagTable.check(tables);
}

void TagDb::insert(const Tag &t)
{
	sql.transaction();
	try {
		tagTable.insert(t);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void TagDb::update(const Tag &orig, const Tag &modified)
{
	sql.transaction();
	try {
		tagTable.update(orig, modified);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void TagDb::del(const Tag &t)
{
	sql.transaction();
	try {
		tagTable.del(t);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void TagDb::query(TagSet &ts)
{
	sql.transaction();
	try {
		tagTable.query(ts);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}
