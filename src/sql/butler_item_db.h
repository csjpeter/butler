/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_DB_H
#define BUTLER_ITEM_DB_H

#include <butler_item_table.h>
#include <butler_itembought_table.h>
#include <butler_tag_db.h>

class ItemDb
{
public:
	ItemDb(SqlConnection &sql, TagDb &tagDb);
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
	SqlConnection &sql;
	TagDb &tagDb;
	ItemBoughtTable itemBoughtTable;
	ItemTable itemTable;
};

#endif
