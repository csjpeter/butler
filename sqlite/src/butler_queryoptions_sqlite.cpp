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

#include "ButlerQueryOptions"
#include "ButlerSqlite"

namespace Butler {

	bool Sqlite::createQueriesTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE Queries ("
				"query_name VARCHAR(64) NOT NULL, "
				"start_date DATE NOT NULL, "
				"end_date DATE NOT NULL"
				")"
				);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkQueriesTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("Queries");
		if(		!table.contains("query_name") ||
				!table.contains("start_date") ||
				!table.contains("end_date")
				) {
			ret = false;
			qCritical("Incompatible table Queries "
					"in the openend database.");
		}
		
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::insertQueryOptions(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "INSERT INTO QueryOptions (query_name, start_date, end_date) VALUES('";
		query += qo.name;
		query += "','";
		query += qo.startDate.toString();
		query += "','";
		query += qo.endDate.toString();
		query += "')";
		sqlQuery.exec(query);
		ret = reportSqlError();

		if(ret)
			ret = insertQueryTags(qo);

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::updateQueryOptions(const QueryOptions &orig, const QueryOptions &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;
		
		int diffs = 0;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "UPDATE QueryOptions SET ";
		if(orig.name != modified.name){
			diffs++;
			query += "name = '";
			query += modified.name;
			query += "'";
		}
		if(orig.startDate != modified.startDate){
			if(diffs)
				query += ", ";
			diffs++;
			query += " start_date = '";
			query += modified.startDate.toString();
			query += "'";
		}
		if(orig.endDate != modified.endDate){
			if(diffs)
				query += ", ";
			diffs++;
			query += "end_date = '";
			query += modified.endDate.toString();
			query += "'";
		}
		query += " WHERE name = '";
		query += orig.name;
		query += "') ";
		if(diffs){
			sqlQuery.exec(query);
			ret = reportSqlError();
		}

		if(ret)
			ret = updateQueryTags(orig, modified);

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::deleteQueryOptions(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "DELETE FROM QueryOptions WHERE name = '";
		query += qo.name;
		query += "')";
		sqlQuery.exec(query);
		ret = reportSqlError();
	
		/* the following should be done automatically by a good SQL server */
		if(ret)
			deleteQueryTags(qo);

		LEAVE_FUNCTION();
		return ret;
	}

	QueryOptions* Sqlite::queryQueryOptions(const QString &name)
	{
		ENTER_FUNCTION();
		Q_UNUSED(name);

		/*	QList<Tag*>* Sqlite::queryQueryOptionTags()
			{
			*/
		/* rewrite this to use Queries and QueryTags table instead */
#if 0
		/* assemble query */
		QString query("SELECT name, active FROM Tags");

		/* execute query */
		QSqlQuery sqlQuery(query, *db);
		sqlQuery.exec();

		/* evaluate query result */
		int nameNo = sqlQuery.record().indexOf("name");
		int activeNo = sqlQuery.record().indexOf("active");

		QList<Tag*> *tags = new QList<Tag*>;

		qDebug("----- Query option tags from db:");

		while (sqlQuery.next()) {
			Tag *tag = new Tag();

			tag->name = QVariant(sqlQuery.value(nameNo).toString());
			if(sqlQuery.value(activeNo).toString() == "t")
				tag->checked = true;
			else
				tag->checked = false;

			tags->append(tag);

			qDebug("Tag: %s", qPrintable(tag->name.toString()));
		}

		qDebug("-----");

		return tags;
#endif
		/*
		   return NULL;
		   }
		   */
		
		LEAVE_FUNCTION();
		return new QueryOptions;
	}
}

