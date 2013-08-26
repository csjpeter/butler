/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_tag_db.h"

TagDb::TagDb(SqlConnection &sql) :
	sql(sql),
	tagTable(sql)
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
	SqlTransaction tr(sql);
	tagTable.insert(t);
	tr.commit();
}

void TagDb::update(const Tag &orig, const Tag &modified)
{
	SqlTransaction tr(sql);
	tagTable.update(orig, modified);
	tr.commit();
}

void TagDb::del(const Tag &t)
{
	SqlTransaction tr(sql);
	tagTable.del(t);
	tr.commit();
}

void TagDb::query(TagSet &ts)
{
	SqlTransaction tr(sql);
	tagTable.query(ts);
	tr.commit();
}
