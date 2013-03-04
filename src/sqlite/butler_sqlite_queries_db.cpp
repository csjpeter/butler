/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_sqlite_sql.h"
#include "butler_sqlite_queries_db.h"

namespace Sqlite {

QueryDb::QueryDb(Sql &_sql, TagDb &tagDb) :
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

bool QueryDb::create()
{
	bool ret;

	ret = queryTable.create();
	ret = ret && queryTagsTable.create();
	ret = ret && queryWaresTable.create();
	ret = ret && queryShopsTable.create();

	return ret;
}

bool QueryDb::check(QStringList &tables)
{
	bool ret;

	ret = queryTable.check(tables);
	ret = ret && queryTagsTable.check(tables);
	ret = ret && queryWaresTable.check(tables);
	ret = ret && queryShopsTable.check(tables);

	return ret;
}

bool QueryDb::update()
{
	return true;
}

bool QueryDb::insert(const Query &q)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && queryTable.insert(q);
	ret = ret && queryTagsTable.insert(q);
	ret = ret && queryWaresTable.insert(q);
	ret = ret && queryShopsTable.insert(q);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool QueryDb::update(const Query &orig, const Query &modified)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && queryTable.update(orig, modified);
	ret = ret && queryTagsTable.update(orig, modified);
	ret = ret && queryWaresTable.update(orig, modified);
	ret = ret && queryShopsTable.update(orig, modified);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool QueryDb::del(const Query &q)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && queryTable.del(q);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool QueryDb::query(QuerySet &qs)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && queryTable.query(qs);

	if(ret){
		unsigned s = qs.size();
		for(unsigned i=0; i<s && ret; i++){
			Query &q = qs.queryAt(i);
			/* FIXME : We need to upgrade the schema for this. */
/*			ret = ret && queryTagsTable.query(q, q.withoutTags);*/
			ret = ret && queryTagsTable.query(q, q.withTags);
			ret = ret && queryWaresTable.query(q, q.wares);
			ret = ret && queryShopsTable.query(q, q.shops);
		}
	}
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

}

