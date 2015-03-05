/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_DB_H
#define BUTLER_ITEM_DB_H

#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class ItemDb
{
public:
	ItemDb(SqlConnection & sql);
	virtual ~ItemDb();
private:
	ItemDb();

public:
	void insert(const Item &);
	void update(const Item & orig, const Item & modified);
	void del(const Item &);
	void query(const TagNameSet &, ItemSet &);
	void query(const Query &, QueryStat &, ItemSet &);

private:
	SqlConnection & sql;
};

#endif
