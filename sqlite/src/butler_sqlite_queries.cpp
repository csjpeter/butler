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

#include "ButlerQuery"
#include "butler_sqlite_queries.h"

namespace Butler {
namespace Sqlite {

	QueryDb::QueryDb(Db &_db, TagDb &_tagDb) :
		db(_db),
		tagDb(_tagDb),
		queryTagsDb(_db, _tagDb)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	QueryDb::~QueryDb()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	bool QueryDb::initializeTables(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret;

		if(!tables.contains("Queries"))
			ret = createQueriesTable();
		else
			ret = checkQueriesTable();

		ret = ret && queryTagsDb.initializeTables(tables);

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryDb::createQueriesTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db.db);
		query.exec("CREATE TABLE Queries ("
				"query_name VARCHAR(64) NOT NULL, "
				"start_date DATE NOT NULL, "
				"end_date DATE NOT NULL"
				")"
				);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryDb::checkQueriesTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.db.record("Queries");
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

	bool QueryDb::insertQuery(const Query &q)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "INSERT INTO Queries (query_name, start_date, end_date) VALUES('";
		query += q.name;
		query += "','";
		query += q.startDate.toString();
		query += "','";
		query += q.endDate.toString();
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		if(ret)
			ret = queryTagsDb.insertQueryTags(q);

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryDb::updateQuery(const Query &orig, const Query &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;
		
		int diffs = 0;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "UPDATE Queries SET ";
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
			ret = db.reportSqlError();
		}

		if(ret)
			ret = queryTagsDb.updateQueryTags(orig, modified);

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryDb::deleteQuery(const Query &q)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM Queries WHERE name = '";
		query += q.name;
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();
	
		/* the following should be done automatically by a good SQL server */
		if(ret)
			queryTagsDb.deleteQueryTags(q);

		LEAVE_FUNCTION();
		return ret;
	}

	Query* QueryDb::queryQuery(const QString &name)
	{
		ENTER_FUNCTION();
		Q_UNUSED(name);

		/*	QList<Tag*>* QueryDb::queryQueryOptionTags()
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
		return new Query;
	}
}
}

