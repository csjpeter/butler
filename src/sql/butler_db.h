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
	Db(const QString& path) :
		sql(path),
		tagDb(sql),
		queryDb(sql, tagDb),
		wareDb(sql, tagDb),
		shopDb(sql),
		itemDb(sql, tagDb)
	{
		QStringList tables = sql.tables();
		priv->tagDb.check(tables);
		priv->queryDb.check(tables);
		priv->shopDb.check(tables);
		priv->wareDb.check(tables);
		priv->itemDb.check(tables);
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
	TagDb tagDb;
	QueryDb queryDb;
	WareDb wareDb;
	ShopDb shopDb;
	ItemDb itemDb;
};

#endif
