/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QVariant>

#include <butler_query.h>

#include "butler_querytags_table.h"

QueryTagsTable::QueryTagsTable(SqlConnection &_sql) :
	sql(_sql)
{
}

QueryTagsTable::~QueryTagsTable()
{
}

void QueryTagsTable::check(QStringList &tables)
{
	if(!tables.contains("query_tags")){
		sql.exec("CREATE TABLE query_tags ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "tag VARCHAR(64) NOT NULL REFERENCES tags(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, tag)"
				  ")"
			    );
		sql.exec("CREATE INDEX query_tags_query_name_index ON query_tags(query_name)");
		sql.exec("CREATE INDEX query_tags_query_tag_index ON query_tags(tag)");
	}

	SqlColumns cols = sql.columns("query_tags");
	if(		!cols.has("query_name") ||
			!cols.has("tag")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table query_tags in the openend database.");
}

void QueryTagsTable::insert(const Query &q, const QString &tagName)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO query_tags "
				"(query_name, tag) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, tagName);
	insertQuery.exec();
}

void QueryTagsTable::del(const Query &q, const QString &tagName)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM query_tags WHERE "
				"query_name = ? AND tag = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, tagName);
	deleteQuery.exec();
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
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, tag FROM query_tags "
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
}
