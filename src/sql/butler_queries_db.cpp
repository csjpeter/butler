/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_queries_db.h"

QueryDb::QueryDb(SqlConnection &sql, TagDb &tagDb) :
	sql(sql),
	tagDb(tagDb),
	queryTable(sql),
	queryTagsTable(sql),
	queryWaresTable(sql),
	queryPartnersTable(sql)
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
	queryPartnersTable.check(tables);
}

void QueryDb::insert(const Query &q)
{
	SqlTransaction tr(sql);
	queryTable.insert(q);
	queryTagsTable.insert(q);
	queryWaresTable.insert(q);
	queryPartnersTable.insert(q);
	tr.commit();
}

void QueryDb::update(const Query &orig, const Query &modified)
{
	SqlTransaction tr(sql);
	queryTable.update(orig, modified);
	queryTagsTable.update(orig, modified);
	queryWaresTable.update(orig, modified);
	queryPartnersTable.update(orig, modified);
	tr.commit();
}

void QueryDb::del(const Query &q)
{
	SqlTransaction tr(sql);
	queryTable.del(q);
	tr.commit();
}

void QueryDb::query(QuerySet &qs)
{
	SqlTransaction tr(sql);
	queryTable.query(qs);

	unsigned s = qs.size();
	for(unsigned i=0; i<s; i++){
		Query &q = qs.queryAt(i);
		/* FIXME : We need to upgrade the schema for this. */
		/* queryTagsTable.query(q, q.withoutTags); */
		queryTagsTable.query(q, q.withTags);
		queryWaresTable.query(q, q.wares);
		queryPartnersTable.query(q, q.partners);
	}
	tr.commit();
}
