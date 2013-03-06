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

void TagDb::check(QStringList &tables)
{
	tagTable.check(tables);
}

void TagDb::insert(const Tag &t)
{
	sql.transaction();
	tagTable.insert(t);
	sql.commit();
}

void TagDb::update(const Tag &orig, const Tag &modified)
{
	sql.transaction();
	tagTable.update(orig, modified);
	sql.commit();
}

void TagDb::del(const Tag &t)
{
	sql.transaction();
	tagTable.del(t);
	sql.commit();
}

void TagDb::query(TagSet &ts)
{
	sql.transaction();
	tagTable.query(ts);
	sql.commit();
}

}

