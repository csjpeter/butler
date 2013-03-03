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

#include <butler_query.h>

#include "butler_sqlite_querytags.h"

namespace Sqlite {

QueryTagsTable::QueryTagsTable(Sql &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

QueryTagsTable::~QueryTagsTable()
{
}

bool QueryTagsTable::create()
{
	bool ret = true;

	ret = ret && sql.exec("CREATE TABLE QueryTags ("
			"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
			"tag VARCHAR(64) NOT NULL REFERENCES Tags(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"UNIQUE (query_name, tag)"
			")"
			);
	ret = ret && sql.exec("CREATE INDEX QueryTagsQueryNameIndex "
			"ON QueryTags(query_name)");
	ret = ret && sql.exec("CREATE INDEX QueryTagsTagIndex "
			"ON QueryTags(tag)");

	return ret;
}

bool QueryTagsTable::check(QStringList &tables)
{
	bool ret;

	ret = tables.contains("QueryTags");

	if(ret){
		QSqlRecord table = sql.record("QueryTags");
		if(		!table.contains("query_name") ||
				!table.contains("tag")
		  ) {
			ret = false;
			LOG("Incompatible table QueryTags in the openend database.");
		}
	}

	return ret;
}

bool QueryTagsTable::insert(const Query &q, const QString &tagName)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		ret = insertQuery.prepare("INSERT INTO QueryTags "
				"(query_name, tag) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, tagName);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool QueryTagsTable::del(const Query &q, const QString &tagName)
{
	bool ret = true;

	if(!deleteQuery.isPrepared())
		ret = deleteQuery.prepare(
				"DELETE FROM QueryTags WHERE "
				"query_name = ? AND tag = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, tagName);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool QueryTagsTable::insert(const Query &q)
{
	bool ret = true;

	unsigned i, s = q.withTags.size();
	for(i=0; i<s; i++){
		const QString &t = q.withTags.queryAt(i);
		ret = insert(q, t);
		if(!ret)
			break;
	}

	return ret;
}

bool QueryTagsTable::update(const Query &orig, const Query &modified)
{
	bool ret = true;

	unsigned i, s = modified.withTags.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.withTags.queryAt(i);
		if(!orig.withTags.has(mTag)){
			ret = insert(modified, mTag);
			if(!ret)
				break;
			continue;
		}
	}

	s = orig.withTags.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.withTags.queryAt(i);
		if(!modified.withTags.has(oTag)){
			ret = del(orig, oTag);
			if(!ret)
				break;
			continue;
		}
	}

	return ret;
}

bool QueryTagsTable::query(const Query &q, TagNameSet &withTags)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		ret = selectQuery.prepare("SELECT query_name, tag FROM QueryTags "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	ret = ret && selectQuery.exec();

	if(ret){
		withTags.clear();

		int tagNo = selectQuery.colIndex("tag");

		DBG("----- Query withTags query result:");
		while (selectQuery.next()) {
			DBG("Next row");
			withTags.add(new QString(selectQuery.value(tagNo).toString()));
		}
		DBG("-----");
	}
	selectQuery.finish();

	return ret;
}

}

