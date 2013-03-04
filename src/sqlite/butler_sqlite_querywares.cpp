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

#include "butler_sqlite_querywares.h"

namespace Sqlite {

QueryWaresTable::QueryWaresTable(Sql &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

QueryWaresTable::~QueryWaresTable()
{
}

bool QueryWaresTable::create()
{
	bool ret = true;

	ret = ret && sql.exec("CREATE TABLE QueryWares ("
			"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
			"ware VARCHAR(64) NOT NULL REFERENCES Wares(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"UNIQUE (query_name, ware)"
			")"
			);
	ret = ret && sql.exec("CREATE INDEX QueryWaresQueryNameIndex "
			"ON QueryWares(query_name)");
	ret = ret && sql.exec("CREATE INDEX QueryWaresTagIndex "
			"ON QueryWares(ware)");

	return ret;
}

bool QueryWaresTable::check(QStringList &tables)
{
	bool ret;

	ret = tables.contains("QueryWares");

	if(ret){
		QSqlRecord table = sql.record("QueryWares");
		if(		!table.contains("query_name") ||
				!table.contains("ware")
		  ) {
			ret = false;
			LOG("Incompatible table QueryWares in the openend database.");
		}
	}

	return ret;
}

bool QueryWaresTable::insert(const Query &q, const QString &wareName)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		ret = insertQuery.prepare("INSERT INTO QueryWares "
				"(query_name, ware) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, wareName);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool QueryWaresTable::del(const Query &q, const QString &wareName)
{
	bool ret = true;

	if(!deleteQuery.isPrepared())
		ret = deleteQuery.prepare(
				"DELETE FROM QueryWares WHERE "
				"query_name = ? AND ware = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, wareName);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool QueryWaresTable::insert(const Query &q)
{
	bool ret = true;

	unsigned i, s = q.wares.size();
	for(i=0; i<s; i++){
		const QString &t = q.wares.queryAt(i);
		ret = insert(q, t);
		if(!ret)
			break;
	}

	return ret;
}

bool QueryWaresTable::update(const Query &orig, const Query &modified)
{
	bool ret = true;

	unsigned i, s = modified.wares.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.wares.queryAt(i);
		if(!orig.wares.has(mTag)){
			ret = insert(modified, mTag);
			if(!ret)
				break;
			continue;
		}
	}

	s = orig.wares.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.wares.queryAt(i);
		if(!modified.wares.has(oTag)){
			ret = del(orig, oTag);
			if(!ret)
				break;
			continue;
		}
	}

	return ret;
}

bool QueryWaresTable::query(const Query &q, WareNameSet &wares)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		ret = selectQuery.prepare("SELECT query_name, ware FROM QueryWares "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	ret = ret && selectQuery.exec();

	if(ret){
		wares.clear();

		int wareNo = selectQuery.colIndex("ware");

		DBG("----- Query wares query result:");
		while (selectQuery.next()) {
			DBG("Next row");
			wares.add(new QString(selectQuery.value(wareNo).toString()));
		}
		DBG("-----");
	}
	selectQuery.finish();

	return ret;
}

}

