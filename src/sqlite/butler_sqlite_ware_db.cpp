/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_sqlite_sql.h"
#include "butler_sqlite_ware_db.h"

namespace Sqlite {

WareDb::WareDb(Sql &_sql, TagDb &tagDb) :
	sql(_sql),
	tagDb(tagDb),
	wareTable(_sql),
	wareTagsTable(_sql),
	wareCategoriesTable(_sql)
{
}

WareDb::~WareDb()
{
}

bool WareDb::create()
{
	bool ret;

	ret = wareTable.create();
	ret = ret && wareTagsTable.create();
	ret = ret && wareCategoriesTable.create();

	return ret;
}

bool WareDb::check(QStringList &tables)
{
	bool ret;

	ret = wareTable.check(tables);
	ret = ret && wareTagsTable.check(tables);
	ret = ret && wareCategoriesTable.check(tables);

	return ret;
}

bool WareDb::update()
{
	return true;
}

bool WareDb::insert(const Ware &ware)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && wareTable.insert(ware);
	ret = ret && wareTagsTable.insert(ware);
	ret = ret && wareCategoriesTable.insert(ware);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool WareDb::update(const Ware &orig, const Ware &modified)
{
	bool ret = true;

	ret = sql.transaction();
	ret = ret && wareTable.update(orig, modified);
	ret = ret && wareTagsTable.update(orig, modified);
	ret = ret && wareCategoriesTable.update(orig, modified);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool WareDb::del(const Ware &w)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && wareTable.del(w);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

bool WareDb::query(WareSet &ws)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && wareTable.query(ws);

	if(ret){
		unsigned i, s = ws.size();
		for(i=0; i<s && ret; i++){
			Ware &ware = ws.queryAt(i);
			ret = ret && wareTagsTable.query(ware, ware.tags);
			ret = ret && wareCategoriesTable.query(ware, ware.categories);
		}
	}
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

}
