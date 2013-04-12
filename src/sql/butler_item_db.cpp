/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item_db.h"

ItemDb::ItemDb(SqlConnection &_sql, TagDb &tagDb) :
	sql(_sql),
	tagDb(tagDb),
	itemBoughtTable(_sql),
	itemTable(_sql)
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
	sql.transaction();
	try{
		itemTable.insert(i);
		if(i.bought)
			itemBoughtTable.insert(i);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ItemDb::update(const Item &orig, const Item &modified)
{
	sql.transaction();
	try {
		itemTable.update(orig, modified);
		if(!orig.bought && modified.bought){
			itemBoughtTable.insert(modified);
		} else if(orig.bought && !modified.bought){
			itemBoughtTable.del(orig);
		} else if(orig.bought && modified.bought){
			itemBoughtTable.update(orig, modified);
		}
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ItemDb::del(const Item &i)
{
	sql.transaction();
	try {
		itemTable.del(i);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ItemDb::query(const TagNameSet &tags, ItemSet &is)
{
	sql.transaction();
	try {
		itemTable.query(tags, is);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void ItemDb::query(const Query &q, QueryStat &stat, ItemSet &is)
{
	sql.transaction();
	try {
		itemBoughtTable.query(q, stat, is);
		unsigned s = is.size();
		for(unsigned i=0; i<s; i++){
			Item &item = is.queryAt(i);
			itemTable.query(item);
		}
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}
