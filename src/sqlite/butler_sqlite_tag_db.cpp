/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_sqlite_sql.h"
#include "butler_sqlite_tag_db.h"

namespace Sqlite {

TagDb::TagDb(Sql &_sql) :
	sql(_sql),
	tagTable(_sql)
{
}

TagDb::~TagDb()
{
}

void TagDb::create()
{
	return tagTable.create();
}

void TagDb::check(QStringList &tables)
{
	return tagTable.check(tables);
}

void TagDb::update()
{
	return true;
}

void TagDb::insert(const Tag &t)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.insert(t);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void TagDb::update(const Tag &orig, const Tag &modified)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.update(orig, modified);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void TagDb::del(const Tag &t)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.del(t);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void TagDb::query(TagSet &ts)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.query(ts);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

}

