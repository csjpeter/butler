/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_sqlite_sql.h"
#include "butler_sqlite_queries_db.h"

QueryDb::QueryDb(SqlConnection &_sql, TagDb &tagDb) :
	sql(_sql),
	tagDb(tagDb),
	queryTable(_sql),
	queryTagsTable(_sql),
	queryWaresTable(_sql),
	queryShopsTable(_sql)
{
}

QueryDb::~QueryDb()
{
}

void QueryDb::check(QStringList &tables)
{
	queryTable.check(tables);
	queryTagsTable.check(tables);
	queryWaresTable.check(tables);
	queryShopsTable.check(tables);
}

void QueryDb::insert(const Query &q)
{
	sql.transaction();
	queryTable.insert(q);
	queryTagsTable.insert(q);
	queryWaresTable.insert(q);
	queryShopsTable.insert(q);
	sql.commit();
}

void QueryDb::update(const Query &orig, const Query &modified)
{
	sql.transaction();
	queryTable.update(orig, modified);
	queryTagsTable.update(orig, modified);
	queryWaresTable.update(orig, modified);
	queryShopsTable.update(orig, modified);
	sql.commit();
}

void QueryDb::del(const Query &q)
{
	sql.transaction();
	queryTable.del(q);
	sql.commit();
}

void QueryDb::query(QuerySet &qs)
{
	sql.transaction();
	queryTable.query(qs);

	unsigned s = qs.size();
	for(unsigned i=0; i<s; i++){
		Query &q = qs.queryAt(i);
		/* FIXME : We need to upgrade the schema for this. */
/*		queryTagsTable.query(q, q.withoutTags);*/
		queryTagsTable.query(q, q.withTags);
		queryWaresTable.query(q, q.wares);
		queryShopsTable.query(q, q.shops);
	}
	sql.commit();
}
