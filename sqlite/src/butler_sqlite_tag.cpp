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
		db(_db),
		insertQuery(0),
		updateQuery(0),
		deleteQuery(0),
		selectOneQuery(0),
		selectAllQuery(0)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	TagDb::~TagDb()
	{
		ENTER_DESTRUCTOR();
		if(!insertQuery)
			delete insertQuery;
		if(!updateQuery)
			delete updateQuery;
		if(!deleteQuery)
			delete deleteQuery;
		if(!selectOneQuery)
			delete selectOneQuery;
		if(!selectAllQuery)
			delete selectAllQuery;
		LEAVE_DESTRUCTOR();
	}

	bool TagDb::create()
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

	bool TagDb::check(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret = true;

		ret = tables.contains("Tags");

		if(ret){
			QSqlRecord table = db.db.record("Tags");
			if(		!table.contains("name")
			  ) {
				ret = false;
				qCritical("Incompatible table Tags "
						"in the openend database.");
			}
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool TagDb::update()
	{
		return true;
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

		if(selectAllQuery == NULL){
			selectAllQuery = new QSqlQuery(db.db);
			selectAllQuery->setForwardOnly(true);
			selectAllQuery->prepare("SELECT name FROM Tags");
		}

		selectAllQuery->exec();

		TagSet *tags = new TagSet();

		v1Debug("----- Reading all tags from db:");
		while (selectAllQuery->next()) {
			Tag *tag = new Tag(selectAllQuery->value(0).toString());
			tags->append(tag);
			v1Debug("Tag: %s", qPrintable(tag->name));
		}
		v1Debug("-----");

		selectAllQuery->finish();

		LEAVE_FUNCTION();
		return tags;
	}
}
}

