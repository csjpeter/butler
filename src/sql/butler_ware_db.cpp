/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_ware_db.h"

WareDb::WareDb(SqlConnection &sql, TagDb &tagDb) :
	sql(sql),
	tagDb(tagDb),
	wareTable(sql),
	wareTagsTable(sql),
	wareCategoriesTable(sql)
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
	SqlTransaction tr(sql);
	wareTable.insert(ware);
	wareTagsTable.insert(ware);
	wareCategoriesTable.insert(ware);
	tr.commit();
}

void WareDb::update(const Ware &orig, const Ware &modified)
{
	SqlTransaction tr(sql);
	wareTable.update(orig, modified);
	wareTagsTable.update(orig, modified);
	wareCategoriesTable.update(orig, modified);
	tr.commit();
}

void WareDb::del(const Ware &w)
{
	SqlTransaction tr(sql);
	wareTable.del(w);
	tr.commit();
}

void WareDb::query(WareSet &ws)
{
	SqlTransaction tr(sql);
	wareTable.query(ws);

	wareTagsTable.query(ws);
	wareCategoriesTable.query(ws);

/*	More precisely with much more queries:
	unsigned i, s = ws.size();
	for(i=0; i<s; i++){
		Ware &ware = ws.queryAt(i);
		wareTagsTable.query(ware, ware.tags);
		wareCategoriesTable.query(ware, ware.categories);
	}*/
	tr.commit();
}
