/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_ware_db.h"

WareDb::WareDb(SqlConnection &_sql, TagDb &tagDb) :
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

void WareDb::check(QStringList &tables)
{
	wareTable.check(tables);
	wareTagsTable.check(tables);
	wareCategoriesTable.check(tables);
}

void WareDb::insert(const Ware &ware)
{
	sql.transaction();
	wareTable.insert(ware);
	wareTagsTable.insert(ware);
	wareCategoriesTable.insert(ware);
	sql.commit();
}

void WareDb::update(const Ware &orig, const Ware &modified)
{
	sql.transaction();
	wareTable.update(orig, modified);
	wareTagsTable.update(orig, modified);
	wareCategoriesTable.update(orig, modified);
	sql.commit();
}

void WareDb::del(const Ware &w)
{
	sql.transaction();
	wareTable.del(w);
	sql.commit();
}

void WareDb::query(WareSet &ws)
{
	sql.transaction();
	wareTable.query(ws);

	unsigned i, s = ws.size();
	for(i=0; i<s; i++){
		Ware &ware = ws.queryAt(i);
		wareTagsTable.query(ware, ware.tags);
		wareCategoriesTable.query(ware, ware.categories);
	}
	sql.commit();
}
