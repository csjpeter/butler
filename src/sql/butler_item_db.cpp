/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item_db.h"

ItemDb::ItemDb(SqlConnection &sql, TagDb &tagDb) :
	sql(sql),
	tagDb(tagDb),
	itemBoughtTable(sql),
	itemTable(sql)
{
}

ItemDb::~ItemDb()
{
}

void ItemDb::check(QStringList &tables)
{
	itemTable.check(tables);
	itemBoughtTable.check(tables);
}

void ItemDb::insert(const Item &i)
{
	SqlTransaction tr(sql);
	itemTable.insert(i);
	if(i.bought)
		itemBoughtTable.insert(i);
	tr.commit();
}

void ItemDb::update(const Item &orig, const Item &modified)
{
	SqlTransaction tr(sql);
	itemTable.update(orig, modified);
	if(!orig.bought && modified.bought){
		itemBoughtTable.insert(modified);
	} else if(orig.bought && !modified.bought){
		itemBoughtTable.del(orig);
	} else if(orig.bought && modified.bought){
		itemBoughtTable.update(orig, modified);
	}
	tr.commit();
}

void ItemDb::del(const Item &i)
{
	SqlTransaction tr(sql);
	itemTable.del(i);
	tr.commit();
}

void ItemDb::query(const TagNameSet &tags, ItemSet &is)
{
	SqlTransaction tr(sql);
	itemTable.query(tags, is);
	tr.commit();
}

void ItemDb::query(const Query &q, QueryStat &stat, ItemSet &is)
{
	SqlTransaction tr(sql);
	itemBoughtTable.query(q, stat, is);
	tr.commit();
}
