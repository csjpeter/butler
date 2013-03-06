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

QueryTagsTable::QueryTagsTable(SqlConnection &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

QueryTagsTable::~QueryTagsTable()
{
}

void QueryTagsTable::check(QStringList &tables)
{
	if(!tables.contains("QueryTags")){
		sql.exec("CREATE TABLE QueryTags ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "tag VARCHAR(64) NOT NULL REFERENCES Tags(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, tag)"
				  ")"
			    );
		sql.exec("CREATE INDEX QueryTagsQueryNameIndex ON QueryTags(query_name)");
		sql.exec("CREATE INDEX QueryTagsTagIndex ON QueryTags(tag)");
	}

	QSqlRecord table = sql.record("QueryTags");
	if(		!table.contains("query_name") ||
			!table.contains("tag")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table QueryTags in the openend database.");
}

void QueryTagsTable::insert(const Query &q, const QString &tagName)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO QueryTags "
				"(query_name, tag) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, tagName);
	insertQuery.exec();
	insertQuery.finish();
}

void QueryTagsTable::del(const Query &q, const QString &tagName)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM QueryTags WHERE "
				"query_name = ? AND tag = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, tagName);
	deleteQuery.exec();
	deleteQuery.finish();
}

void QueryTagsTable::insert(const Query &q)
{
	unsigned i, s = q.withTags.size();
	for(i=0; i<s; i++){
		const QString &t = q.withTags.queryAt(i);
		insert(q, t);
	}
}

void QueryTagsTable::update(const Query &orig, const Query &modified)
{
	unsigned i, s = modified.withTags.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.withTags.queryAt(i);
		if(!orig.withTags.has(mTag))
			insert(modified, mTag);
	}

	s = orig.withTags.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.withTags.queryAt(i);
		if(!modified.withTags.has(oTag))
			del(orig, oTag);
	}
}

void QueryTagsTable::query(const Query &q, TagNameSet &withTags)
{
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, tag FROM QueryTags "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	selectQuery.exec();

	withTags.clear();

	int tagNo = selectQuery.colIndex("tag");

	DBG("----- Query withTags query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		withTags.add(new QString(selectQuery.value(tagNo).toString()));
	}
	DBG("-----");
	selectQuery.finish();
}
