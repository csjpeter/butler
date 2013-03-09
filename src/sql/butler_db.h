/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DB_H
#define BUTLER_DB_H

#include <butler_sql_connection.h>
#include <butler_tag_db.h>
#include <butler_queries_db.h>
#include <butler_item_db.h>
#include <butler_ware_db.h>
#include <butler_shop_db.h>
#include <butler_db.h>

class Private;

class Db
{
public:
	Db(const DatabaseDescriptor & dbDesc) :
		sql(dbDesc),
		desc(dbDesc),
		tag(sql),
		query(sql, tag),
		ware(sql, tag),
		shop(sql),
		item(sql, tag)
	{
		QStringList tables = sql.tables();
		tag.check(tables);
		query.check(tables);
		shop.check(tables);
		ware.check(tables);
		item.check(tables);
	}
	~Db() { }

public:
#if 0
	void open() { sql.open(); }
	void close() { sql.close(); }
#endif

private:
	SqlConnection sql;
public:
	const DatabaseDescriptor & desc;
	TagDb tag;
	QueryDb query;
	WareDb ware;
	ShopDb shop;
	ItemDb item;
};

#endif
