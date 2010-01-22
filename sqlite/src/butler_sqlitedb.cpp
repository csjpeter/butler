/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <ButlerDebug>
#include "butler_sqlite_schemaversion.h"
#include "butler_sqlite_tag.h"
#include "butler_sqlite_queries.h"
#include "butler_sqlite_item.h"
#include "butler_sqlitedb.h"

namespace Butler {
using namespace Butler::Sqlite;

	class Private
	{
		public:
			Private(const QString &path);
			~Private();
		public:
			Sqlite::Db db;
			Sqlite::TagDb tagDb;
			Sqlite::QueryDb queryDb;
			Sqlite::ItemDb itemDb;
	};

	Private::Private(const QString &path) :
		db(path),
		tagDb(db),
		queryDb(db, tagDb),
		itemDb(db, tagDb)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	Private::~Private()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	SqliteDb::SqliteDb(const QString& _path)
	{
		ENTER_CONSTRUCTOR();
		priv = new Private(_path);
		LEAVE_CONSTRUCTOR();
	}

	SqliteDb::~SqliteDb()
	{
		ENTER_DESTRUCTOR();
		delete priv;
		LEAVE_DESTRUCTOR();
	}
	
	bool SqliteDb::connect()
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->db.connect();

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::open()
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->db.open();

		if(ret){
			SchemaVersion ver;
			SchemaVersionDb verDb(priv->db);
			ver = verDb.querySchemaVersion();
			if(
			(ver.major != BUTLER_SQLITE_SCHEMA_VERSION_MAJOR) ||
			(ver.minor < BUTLER_SQLITE_SCHEMA_VERSION_MINOR)
			){
				ret = false;
				priv->db.lastUserErrId =
					INCOMPATIBLE_DATABASE_SCHEMA;
				priv->db.lastUserErr = "Incompatible database "
					"schema version in db";
				priv->db.close();
			}
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::close()
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->db.close();

		LEAVE_FUNCTION();
		return ret;
	}
	
	bool SqliteDb::create()
	{
		ENTER_FUNCTION();

		/* do not create anything on an already opened db */
		bool ret = priv->db.db.isOpen() ? false : true;

		ret = ret && priv->db.open();
		ret = ret && priv->tagDb.create();
/*		ret = ret && priv->queryDb.create();
		ret = ret && priv->itemDb.create();*/
		ret = priv->db.close() && ret;

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::check()
	{
		ENTER_FUNCTION();
		bool ret = priv->db.db.isOpen();

		QStringList tables = priv->db.db.tables();

		ret = ret && priv->tagDb.check(tables);
/*		ret = ret && priv->queryDb.check(tables);
		ret = ret && priv->itemDb.check(tables);*/

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::update()
	{
		ENTER_FUNCTION();
		bool ret = priv->db.db.isOpen() ? false : true;

		ret = ret && priv->db.open();
		ret = ret && priv->tagDb.update();
/*		ret = ret && priv->queryDb.update();
		ret = ret && priv->itemDb.update();*/
		ret = priv->db.close() && ret;

		LEAVE_FUNCTION();
		return ret;
	}

	enum Butler::UserDbError SqliteDb::lastUserErrorId()
	{
		ENTER_FUNCTION();

		enum UserDbError ret = priv->db.lastUserErrorId();

		LEAVE_FUNCTION();
		return ret;
	}

	const QString& SqliteDb::lastUserError()
	{
		ENTER_FUNCTION();

		const QString &ret = priv->db.lastUserError();

		LEAVE_FUNCTION();
		return ret;
	}

	const QString& SqliteDb::lastError()
	{
		ENTER_FUNCTION();

		const QString &ret = priv->db.lastError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::insertTag(const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->tagDb.insertTag(t);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::updateTag(const Tag &orig, const Tag &modified)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->tagDb.updateTag(orig, modified);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::deleteTag(const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->tagDb.deleteTag(t);

		LEAVE_FUNCTION();
		return ret;
	}

	TagSet* SqliteDb::queryTags()
	{
		ENTER_FUNCTION();
		TagSet *ret;

		ret = priv->tagDb.queryTags();

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::insertQuery(const Query &q)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->queryDb.insertQuery(q);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::updateQuery(
			const Query &orig,
			const Query &modified)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->queryDb.updateQuery(orig, modified);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::deleteQuery(const Query &q)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->queryDb.deleteQuery(q);

		LEAVE_FUNCTION();
		return ret;
	}

	TagSet* SqliteDb::queryQueryTags(const Query &q)
	{
		ENTER_FUNCTION();
		TagSet *ret;

		Q_UNUSED(q);
		ret = false;
		/* FIXME may be no need to be part of ifc */
/*		ret = priv->db.queryTags(q);*/

		LEAVE_FUNCTION();
		return ret;
	}

	Query* SqliteDb::queryQuery(const QString &name)
	{
		ENTER_FUNCTION();
		Query *ret;

		ret = priv->queryDb.queryQuery(name);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::insertItem(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->itemDb.insertItem(i);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::updateItem(
			const Item &orig,
			const Item &modified)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->itemDb.updateItem(orig, modified);

		LEAVE_FUNCTION();
		return ret;
	}

	bool SqliteDb::deleteItem(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret;

		ret = priv->itemDb.deleteItem(i);

		LEAVE_FUNCTION();
		return ret;
	}

	TagSet* SqliteDb::queryItemTags(const Item &item)
	{
		ENTER_FUNCTION();
		TagSet *ret;

		ret = false;
		Q_UNUSED(item);
		/* FIXME may be no need to be part of ifc */
/*		ret = priv->db.queryTags(item);*/

		LEAVE_FUNCTION();
		return ret;
	}

	ItemSet* SqliteDb::queryItems(const Query &q)
	{
		ENTER_FUNCTION();
		ItemSet *ret;

		ret = priv->itemDb.queryItems(q);

		LEAVE_FUNCTION();
		return ret;
	}
}

