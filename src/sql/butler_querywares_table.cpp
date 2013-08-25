/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QVariant>

#include <butler_query.h>

#include "butler_querywares_table.h"

QueryWaresTable::QueryWaresTable(SqlConnection &_sql) :
	sql(_sql)
{
}

QueryWaresTable::~QueryWaresTable()
{
}

void QueryWaresTable::check(QStringList &tables)
{
	if(!tables.contains("query_wares")){
		sql.exec("CREATE TABLE query_wares ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "ware VARCHAR(64) NOT NULL REFERENCES wares(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, ware)"
				  ")"
			    );
		sql.exec("CREATE INDEX query_wares_query_name_index ON query_wares(query_name)");
		sql.exec("CREATE INDEX query_wares_tag_index ON query_wares(ware)");
	}

	SqlColumns cols = sql.columns("query_wares");
	if(		!cols.has("query_name") ||
			!cols.has("ware")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table query_wares in the openend database.");
}

void QueryWaresTable::insert(const Query &q, const QString &wareName)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO query_wares "
				"(query_name, ware) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, wareName);
	insertQuery.exec();
}

void QueryWaresTable::del(const Query &q, const QString &wareName)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM query_wares WHERE "
				"query_name = ? AND ware = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, wareName);
	deleteQuery.exec();
}

void QueryWaresTable::insert(const Query &q)
{
	unsigned i, s = q.wares.size();
	for(i=0; i<s; i++){
		const QString &t = q.wares.queryAt(i);
		insert(q, t);
	}
}

void QueryWaresTable::update(const Query &orig, const Query &modified)
{
	unsigned i, s = modified.wares.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.wares.queryAt(i);
		if(!orig.wares.has(mTag))
			insert(modified, mTag);
	}

	s = orig.wares.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.wares.queryAt(i);
		if(!modified.wares.has(oTag))
			del(orig, oTag);
	}
}

void QueryWaresTable::query(const Query &q, WareNameSet &wares)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, ware FROM query_wares "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	selectQuery.exec();

	wares.clear();

	int wareNo = selectQuery.colIndex("ware");

	DBG("----- Query wares query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		wares.add(new QString(selectQuery.value(wareNo).toString()));
	}
	DBG("-----");
}
