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

	bool Sqlite::createQueryTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE QueryTags ("
				"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkQueryTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("QueryTags");
		if(		!table.contains("query_name") ||
				!table.contains("tag")
				) {
			ret = false;
			qCritical("Incompatible table QueryTags "
					"in the openend database.");
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::insertQueryTag(const QueryOptions &qo, const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "INSERT INTO QueryTags (query_name, tag) VALUES('";
		query += qo.name;
		query += "', '";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::deleteQueryTag(const QueryOptions &qo, const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db);
		QString query;
		query = "DELETE FROM QueryTags WHERE query_name = '";
		query += qo.name;
		query += "' AND tag = '";
		query += t.name;
		query += "'";
		sqlQuery.exec(query);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::insertQueryTags(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		bool ret;

		int i, s = qo.tags.size();
		for(i=0; i<s; i++){
			const Tag &t = qo.tags.query(i);
			if(!t.checked)
				continue;
			ret = insertQueryTag(qo, t);
			if(!ret)
				break;
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::updateQueryTags(
			const QueryOptions &orig, const QueryOptions &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlQuery sqlQuery(db);
		QString query;

		int i, s = modified.tags.size();
		for(i=0; i<s; i++){
			const Tag &m = modified.tags.query(i);
			if(!orig.tags.hasByName(m.name) && m.checked){
				ret = insertQueryTag(modified, m);
				if(!ret)
					break;
				continue;
			}

			const Tag &o = orig.tags.queryByName(m.name);
			if(o.checked == m.checked)
				continue;

			if(m.checked){
				ret = insertQueryTag(modified, m);
				if(!ret)
					break;
			} else { /* o.checked is true here */
				ret = deleteQueryTag(orig, o);
				if(!ret)
					break;
			}
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::deleteQueryTags(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		bool ret;
	
		QSqlQuery sqlQuery(db);
		QString query;
		query = "DELETE FROM QueryTags WHERE query_name = '";
		query += qo.name;
		query += "'";
		sqlQuery.exec(query);
		ret = reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}


	TagSet* Sqlite::queryTags(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		Q_UNUSED(qo);

		TagSet *ret = new TagSet;

		LEAVE_FUNCTION();
		return ret;
	}

}

