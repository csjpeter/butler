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
#include "butler_sqlite_querytags.h"

namespace Butler {
namespace Sqlite {

	QueryTagsDb::QueryTagsDb(Db &_db, TagDb &_tagDb) :
		db(_db),
		tagDb(_tagDb)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	QueryTagsDb::~QueryTagsDb()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	bool QueryTagsDb::initializeTables(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret;

		if(!tables.contains("QueryTags"))
			ret = createQueryTagsTable();
		else
			ret = checkQueryTagsTable();

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryTagsDb::createQueryTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db.db);
		query.exec("CREATE TABLE QueryTags ("
				"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryTagsDb::checkQueryTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.db.record("QueryTags");
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

	bool QueryTagsDb::insertQueryTag(const Query &q, const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "INSERT INTO QueryTags (query_name, tag) VALUES('";
		query += q.name;
		query += "', '";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryTagsDb::deleteQueryTag(const Query &q, const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM QueryTags WHERE query_name = '";
		query += q.name;
		query += "' AND tag = '";
		query += t.name;
		query += "'";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryTagsDb::insertQueryTags(const Query &q)
	{
		ENTER_FUNCTION();
		bool ret;

		int i, s = q.tags.size();
		for(i=0; i<s; i++){
			const Tag &t = q.tags.query(i);
			if(!t.checked)
				continue;
			ret = insertQueryTag(q, t);
			if(!ret)
				break;
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool QueryTagsDb::updateQueryTags(
			const Query &orig, const Query &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlQuery sqlQuery(db.db);
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

	bool QueryTagsDb::deleteQueryTags(const Query &q)
	{
		ENTER_FUNCTION();
		bool ret;
	
		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM QueryTags WHERE query_name = '";
		query += q.name;
		query += "'";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}


	TagSet* QueryTagsDb::queryQueryTags(const Query &q)
	{
		ENTER_FUNCTION();
		Q_UNUSED(q);

		TagSet *ret = new TagSet;

		LEAVE_FUNCTION();
		return ret;
	}
}
}

