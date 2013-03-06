/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

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

void ItemDb::check(QStringList &tables)
{
	itemTable.check(tables);
	itemBoughtTable.check(tables);
}

void ItemDb::insert(const Item &i)
{
	sql.transaction();
	itemTable.insert(i);
	if(i.bought)
		itemBoughtTable.insert(i);
	sql.commit();
}

void ItemDb::update(const Item &orig, const Item &modified)
{
	sql.transaction();
	itemTable.update(orig, modified);
	if(!orig.bought && modified.bought){
		itemBoughtTable.insert(modified);
	} else if(orig.bought && !modified.bought){
		itemBoughtTable.del(orig);
	} else if(orig.bought && modified.bought){
		itemBoughtTable.update(orig, modified);
	}
	sql.commit();
}

void ItemDb::del(const Item &i)
{
	sql.transaction();
	itemTable.del(i);
	sql.commit();
}

void ItemDb::query(const TagNameSet &tags, ItemSet &is)
{
	sql.transaction();
	itemTable.query(tags, is);
	sql.commit();
}

void ItemDb::query(const Query &q, QueryStat &stat, ItemSet &is)
{
	sql.transaction();
	itemBoughtTable.query(q, stat, is);

	unsigned s = is.size();
	for(unsigned i=0; i<s; i++){
		Item &item = is.queryAt(i);
		itemTable.query(item);
	}

	sql.commit();
}

}

