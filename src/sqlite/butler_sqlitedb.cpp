/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QSqlError>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_tag_db.h"
#include "butler_sqlite_queries_db.h"
#include "butler_sqlite_item_db.h"
#include "butler_sqlite_ware_db.h"
#include "butler_sqlite_shop_db.h"
#include "butler_sqlitedb.h"

class Private
{
public:
	Private(const QString &path);
	~Private();
public:
	Sqlite::Sql sql;
	Sqlite::TagDb tagDb;
	Sqlite::QueryDb queryDb;
	Sqlite::WareDb wareDb;
	Sqlite::ShopDb shopDb;
	Sqlite::ItemDb itemDb;
};

Private::Private(const QString &path) :
	sql(path),
	tagDb(sql),
	queryDb(sql, tagDb),
	wareDb(sql, tagDb),
	shopDb(sql),
	itemDb(sql, tagDb)
{
}

Private::~Private()
{
}

SqliteDb::SqliteDb(const QString& _path)
{
	priv = new Private(_path);
}

SqliteDb::~SqliteDb()
{
	delete priv;
}

void SqliteDb::connect()
{
	priv->sql.connect();
}

void SqliteDb::open()
{
	priv->sql.open();
}

void SqliteDb::close()
{
	priv->sql.close();
}

void SqliteDb::create()
{
	if(priv->sql.isOpen())
		throw DbError("Can not create database and or tables "
				"in an already opened database.");

	priv->sql.open();

	Sql::Transaction transaction;
	ret = ret && priv->tagDb.create();
	ret = ret && priv->queryDb.create();
	ret = ret && priv->shopDb.create();
	ret = ret && priv->wareDb.create();
	ret = ret && priv->itemDb.create();
	transaction.commit();

	ret = priv->sql.close() && ret;
}

void SqliteDb::check()
{
	bool ret = priv->sql.isOpen();

	QStringList tables = priv->sql.tables();

	ret = ret && priv->tagDb.check(tables);
	ret = ret && priv->queryDb.check(tables);
	ret = ret && priv->shopDb.check(tables);
	ret = ret && priv->wareDb.check(tables);
	ret = ret && priv->itemDb.check(tables);

	return ret;
}

void SqliteDb::update()
{
	bool ret = priv->sql.isOpen() ? false : true;

	ret = ret && priv->sql.open();

	ret = ret && priv->sql.transaction();
	ret = ret && priv->tagDb.update();
	ret = ret && priv->queryDb.update();
	ret = ret && priv->shopDb.update();
	ret = ret && priv->wareDb.update();
	ret = ret && priv->itemDb.update();
	ret = (ret && priv->sql.commit()) || priv->sql.rollback();

	ret = priv->sql.close() && ret;

	return ret;
}

TagDb& SqliteDb::tag()
{
	return priv->tagDb;
}

QueryDb& SqliteDb::query()
{
	return priv->queryDb;
}

ItemDb& SqliteDb::item()
{
	return priv->itemDb;
}

WareDb& SqliteDb::ware()
{
	return priv->wareDb;
}

ShopDb& SqliteDb::shop()
{
	return priv->shopDb;
}
