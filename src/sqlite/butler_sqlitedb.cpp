/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QSqlError>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_schemaversion.h"
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
	Sqlite::SchemaVersion schemaVersion;
	Sqlite::TagDb tagDb;
	Sqlite::QueryDb queryDb;
	Sqlite::WareDb wareDb;
	Sqlite::ShopDb shopDb;
	Sqlite::ItemDb itemDb;
};

Private::Private(const QString &path) :
	sql(path),
	schemaVersion(sql),
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

bool SqliteDb::connect()
{
	return priv->sql.connect();
}

bool SqliteDb::open()
{
	bool ret;

	ret = priv->sql.open();

	if(ret){
		Sqlite::Version ver;
		ret = priv->schemaVersion.query(ver);
		if(
				(ver.major != BUTLER_SQLITE_SCHEMA_VERSION_MAJOR) ||
				(ver.minor < BUTLER_SQLITE_SCHEMA_VERSION_MINOR)
		  ){
			ret = false;
			priv->sql.lastUserErrId =
				INCOMPATIBLE_DATABASE_SCHEMA;
			priv->sql.lastUserErr = "Incompatible database "
				"schema version in sql";
			priv->sql.close();
		}
	}

	return ret;
}

bool SqliteDb::close()
{
	return priv->sql.close();
}

bool SqliteDb::create()
{
	/* do not create anything on an already opened sql */
	bool ret = priv->sql.isOpen() ? false : true;

	ret = ret && priv->sql.open();
	if(ret){
		ret = priv->sql.transaction();
		ret = ret && priv->schemaVersion.create();
		ret = ret && priv->tagDb.create();
		ret = ret && priv->queryDb.create();
		ret = ret && priv->shopDb.create();
		ret = ret && priv->wareDb.create();
		ret = ret && priv->itemDb.create();
		ret = (ret && priv->sql.commit()) || priv->sql.rollback();

		ret = priv->sql.close() && ret;
	}

	return ret;
}

bool SqliteDb::check()
{
	bool ret = priv->sql.isOpen();

	QStringList tables = priv->sql.tables();

	ret = ret && priv->schemaVersion.check(tables);
	ret = ret && priv->tagDb.check(tables);
	ret = ret && priv->queryDb.check(tables);
	ret = ret && priv->shopDb.check(tables);
	ret = ret && priv->wareDb.check(tables);
	ret = ret && priv->itemDb.check(tables);

	return ret;
}

bool SqliteDb::update()
{
	bool ret = priv->sql.isOpen() ? false : true;

	ret = ret && priv->sql.open();

	ret = ret && priv->sql.transaction();
	ret = ret && priv->schemaVersion.update();
	ret = ret && priv->tagDb.update();
	ret = ret && priv->queryDb.update();
	ret = ret && priv->shopDb.update();
	ret = ret && priv->wareDb.update();
	ret = ret && priv->itemDb.update();
	ret = (ret && priv->sql.commit()) || priv->sql.rollback();

	ret = priv->sql.close() && ret;

	return ret;
}

enum Db::UserError SqliteDb::lastUserErrorId()
{
	enum Db::UserError ret = priv->sql.lastUserErrorId();
	return ret;
}

const QString& SqliteDb::lastUserError()
{
	return priv->sql.lastUserError();
}

const QString& SqliteDb::lastError()
{
	return priv->sql.lastError();
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
