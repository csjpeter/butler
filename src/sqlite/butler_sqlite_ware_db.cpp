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

void WareDb::create()
{
	bool ret;

	ret = wareTable.create();
	ret = ret && wareTagsTable.create();
	ret = ret && wareCategoriesTable.create();

	return ret;
}

void WareDb::check(QStringList &tables)
{
	bool ret;

	ret = wareTable.check(tables);
	ret = ret && wareTagsTable.check(tables);
	ret = ret && wareCategoriesTable.check(tables);

	return ret;
}

void WareDb::update()
{
	return true;
}

void WareDb::insert(const Ware &ware)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && wareTable.insert(ware);
	ret = ret && wareTagsTable.insert(ware);
	ret = ret && wareCategoriesTable.insert(ware);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void WareDb::update(const Ware &orig, const Ware &modified)
{
	bool ret = true;

	ret = sql.transaction();
	ret = ret && wareTable.update(orig, modified);
	ret = ret && wareTagsTable.update(orig, modified);
	ret = ret && wareCategoriesTable.update(orig, modified);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void WareDb::del(const Ware &w)
{
	bool ret;

	ret = sql.transaction();
	ret = ret && wareTable.del(w);
	ret = (ret && sql.commit()) || (sql.rollback() && false);

	return ret;
}

void WareDb::query(WareSet &ws)
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
