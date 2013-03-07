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

#include "butler_querywares_table.h"

QueryWaresTable::QueryWaresTable(SqlConnection &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

QueryWaresTable::~QueryWaresTable()
{
}

void QueryWaresTable::check(QStringList &tables)
{
	if(tables.contains("QueryWares")){
		sql.exec("CREATE TABLE QueryWares ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "ware VARCHAR(64) NOT NULL REFERENCES Wares(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, ware)"
				  ")"
			    );
		sql.exec("CREATE INDEX QueryWaresQueryNameIndex ON QueryWares(query_name)");
		sql.exec("CREATE INDEX QueryWaresTagIndex ON QueryWares(ware)");
	}

	QSqlRecord table = sql.record("QueryWares");
	if(		!table.contains("query_name") ||
			!table.contains("ware")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table QueryWares in the openend database.");
}

void QueryWaresTable::insert(const Query &q, const QString &wareName)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO QueryWares "
				"(query_name, ware) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, wareName);
	insertQuery.exec();
	insertQuery.finish();
}

void QueryWaresTable::del(const Query &q, const QString &wareName)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM QueryWares WHERE "
				"query_name = ? AND ware = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, wareName);
	deleteQuery.exec();
	deleteQuery.finish();
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
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, ware FROM QueryWares "
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

	selectQuery.finish();
}
