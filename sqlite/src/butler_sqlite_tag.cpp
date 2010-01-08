/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <ButlerDebug>
#include "butler_sqlite_tag.h"

namespace Butler {
namespace Sqlite {

	TagDb::TagDb(Db &_db) :
		db(_db)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	TagDb::~TagDb()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	bool TagDb::initializeTables(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret;

		if(!tables.contains("Tags"))
			ret = createTagsTable() && ret;
		else
			ret = checkTagsTable() && ret;

		LEAVE_FUNCTION();
		return ret;
	}

	bool TagDb::createTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db.db);
		query.exec("CREATE TABLE Tags ("
				"name VARCHAR(32) PRIMARY KEY "
				")"
				);
		ret = db.reportSqlError();
		LEAVE_FUNCTION();
		return ret;
	}

	bool TagDb::checkTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.db.record("Tags");
		if(		!table.contains("name")
				) {
			ret = false;
			qCritical("Incompatible table Tags "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool TagDb::insertTag(const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "INSERT INTO Tags VALUES('";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool TagDb::updateTag(const Tag &orig, const Tag &modified)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "UPDATE Tags SET name = '";
		query += orig.name;
		query += "' ";
		query += "WHERE name = '";
		query += modified.name;
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool TagDb::deleteTag(const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM Tags WHERE name = '";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	TagSet* TagDb::queryTags()
	{
		ENTER_FUNCTION();
		QString query("SELECT name FROM Tags");

		QSqlQuery sqlQuery(query, db.db);
		sqlQuery.exec();

		int nameNo = sqlQuery.record().indexOf("name");

		TagSet *tags = new TagSet();

		v1Debug("----- Reading all tags from db:");
		while (sqlQuery.next()) {
			Tag *tag = new Tag(sqlQuery.value(nameNo).toString());
			tags->append(tag);
			v1Debug("Tag: %s", qPrintable(tag->name));
		}
		v1Debug("-----");

		LEAVE_FUNCTION();
		return tags;
	}
}
}

