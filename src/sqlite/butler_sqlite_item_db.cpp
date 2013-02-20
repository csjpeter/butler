/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <csjp_debug.h>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_item_db.h"

namespace Sqlite {

ItemDb::ItemDb(Sql &_sql, TagDb &tagDb) :
	sql(_sql),
	tagDb(tagDb),
	itemBoughtTable(_sql),
	itemTable(_sql)
{
}

ItemDb::~ItemDb()
{
}

bool ItemDb::create()
{
	bool ret;

	ret = itemTable.create();
	ret = ret && itemBoughtTable.create();

	return ret;
}

bool ItemDb::check(QStringList &tables)
{
	bool ret;

	ret = itemTable.check(tables);
	ret = ret && itemBoughtTable.check(tables);

	return ret;
}

bool ItemDb::update()
{
	return true;
}

bool ItemDb::insert(const Item &i)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && itemTable.insert(i);
	if(i.bought)
		ret = ret && itemBoughtTable.insert(i);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool ItemDb::update(const Item &orig, const Item &modified)
{
	bool ret = true;

	ret = sql.transaction();
	ret = ret && itemTable.update(orig, modified);
	if(!orig.bought && modified.bought){
		ret = ret && itemBoughtTable.insert(modified);
	} else if(orig.bought && !modified.bought){
		ret = ret && itemBoughtTable.del(orig);
	} else if(orig.bought && modified.bought){
		ret = ret && itemBoughtTable.update(orig, modified);
	}
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool ItemDb::del(const Item &i)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && itemTable.del(i);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool ItemDb::query(const TagNameSet &tags, ItemSet &is)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && itemTable.query(tags, is);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool ItemDb::query(const Query &q, QueryStat &stat, ItemSet &is)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && itemBoughtTable.query(q, stat, is);

	unsigned s = is.size();
	for(unsigned i=0; i<s && ret; i++){
		Item &item = is.queryAt(i);
		ret = ret && itemTable.query(item);
	}

	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

}

