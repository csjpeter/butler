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

Private::~Private() {}

SqliteDb::SqliteDb(const QString& _path) { priv = new Private(_path); }
SqliteDb::~SqliteDb() { delete priv; }
void SqliteDb::connect() { priv->sql.connect(); }
void SqliteDb::open() { priv->sql.open(); }
void SqliteDb::close() { priv->sql.close(); }

void SqliteDb::check()
{
	QStringList tables = priv->sql.tables();

	priv->tagDb.check(tables);
	priv->queryDb.check(tables);
	priv->shopDb.check(tables);
	priv->wareDb.check(tables);
	priv->itemDb.check(tables);
}

enum Db::UserError SqliteDb::lastUserErrorId() { enum Db::UserError ret =
	priv->sql.lastUserErrorId(); return ret; }
const QString& SqliteDb::lastUserError() { return priv->sql.lastUserError(); }
const QString& SqliteDb::lastError() { return priv->sql.lastError(); }
TagDb& SqliteDb::tag() { return priv->tagDb; }
QueryDb& SqliteDb::query() { return priv->queryDb; }
ItemDb& SqliteDb::item() { return priv->itemDb; }
WareDb& SqliteDb::ware() { return priv->wareDb; }
ShopDb& SqliteDb::shop() { return priv->shopDb; }
