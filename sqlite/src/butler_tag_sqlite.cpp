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
#include <ButlerSqlite>

namespace Butler {

	bool Sqlite::createTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE Tags ("
				"name VARCHAR(32) PRIMARY KEY "
				")"
				);
		ret = reportSqlError();
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("Tags");
		if(		!table.contains("name")
				) {
			ret = false;
			qCritical("Incompatible table Tags "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::insertTag(const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "INSERT INTO Tags VALUES('";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::updateTag(const Tag &orig, const Tag &modified)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "UPDATE Tags SET name = '";
		query += orig.name;
		query += "' ";
		query += "WHERE name = '";
		query += modified.name;
		query += "')";
		sqlQuery.exec(query);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::deleteTag(const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "DELETE FROM Tags WHERE name = '";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	TagSet* Sqlite::queryTags()
	{
		ENTER_FUNCTION();
		QString query("SELECT name FROM Tags");

		QSqlQuery sqlQuery(query, db);
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

