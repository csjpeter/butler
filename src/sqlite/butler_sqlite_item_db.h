/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_ITEM_DB_H
#define BUTLER_SQLITE_ITEM_DB_H

#include <butler_db.h>

#include "butler_sqlite_item.h"
#include "butler_sqlite_itembought.h"
#include "butler_sqlite_tag_db.h"

namespace Sqlite {

class ItemDb : public ::ItemDb
{
public:
	ItemDb(Sql &sql, TagDb &tagDb);
	virtual ~ItemDb();
private:
	ItemDb();

public:
	void check(QStringList &tables);

	void insert(const Item &);
	void update(const Item &orig, const Item &modified);
	void del(const Item &);
	void query(const TagNameSet &, ItemSet &);
	void query(const Query &, QueryStat &, ItemSet &);

private:
	Sql &sql;
	TagDb &tagDb;
	ItemBoughtTable itemBoughtTable;
	ItemTable itemTable;
};

}

#endif

