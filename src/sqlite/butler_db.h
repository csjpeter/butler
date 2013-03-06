/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITEDB_H
#define BUTLER_SQLITEDB_H

#include <butler_sqlite_sql.h>
#include <butler_sqlite_tag_db.h>
#include <butler_sqlite_queries_db.h>
#include <butler_sqlite_item_db.h>
#include <butler_sqlite_ware_db.h>
#include <butler_sqlite_shop_db.h>
#include <butler_sqlitedb.h>

class Private;

class Db
{
public:
	Db::Db(SqlConnection & sql) :
		sql(sql),
		tagDb(sql),
		queryDb(sql, tagDb),
		wareDb(sql, tagDb),
		shopDb(sql),
		itemDb(sql, tagDb)
	{ }
	~Db() { }

public:
#if 0
	void open() { sql.open(); }
	void close() { sql.close(); }
#endif
	void check()
	{
		QStringList tables = sql.tables();
		priv->tagDb.check(tables);
		priv->queryDb.check(tables);
		priv->shopDb.check(tables);
		priv->wareDb.check(tables);
		priv->itemDb.check(tables);
	}

private:
	SqlConnection sql;
public:
	TagDb tagDb;
	QueryDb queryDb;
	WareDb wareDb;
	ShopDb shopDb;
	ItemDb itemDb;
};

#endif
