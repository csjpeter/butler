/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

bool TagDb::create()
{
	return tagTable.create();
}

bool TagDb::check(QStringList &tables)
{
	return tagTable.check(tables);
}

bool TagDb::update()
{
	return true;
}

bool TagDb::insert(const Tag &t)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.insert(t);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool TagDb::update(const Tag &orig, const Tag &modified)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.update(orig, modified);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool TagDb::del(const Tag &t)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.del(t);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool TagDb::query(TagSet &ts)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && tagTable.query(ts);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

}

