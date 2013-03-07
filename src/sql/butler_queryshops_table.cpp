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

#include "butler_queryshops_table.h"

QueryShopsTable::QueryShopsTable(SqlConnection &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

QueryShopsTable::~QueryShopsTable()
{
}


void QueryShopsTable::check(QStringList &tables)
{
	if(!tables.contains("QueryShops")){
		sql.exec("CREATE TABLE QueryShops ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "shop VARCHAR(64) NOT NULL REFERENCES Shops(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, shop)"
				  ")"
			    );
		sql.exec("CREATE INDEX QueryShopsQueryNameIndex ON QueryShops(query_name)");
		sql.exec("CREATE INDEX QueryShopsTagIndex ON QueryShops(shop)");
	}

	QSqlRecord table = sql.record("QueryShops");
	if(		!table.contains("query_name") ||
			!table.contains("shop")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table QueryShops in the openend database.");
}

void QueryShopsTable::insert(const Query &q, const QString &shopName)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO QueryShops "
				"(query_name, shop) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, shopName);
	insertQuery.exec();
	insertQuery.finish();
}

void QueryShopsTable::del(const Query &q, const QString &shopName)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM QueryShops WHERE "
				"query_name = ? AND shop = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, shopName);
	deleteQuery.exec();
	deleteQuery.finish();
}

void QueryShopsTable::insert(const Query &q)
{
	unsigned i, s = q.shops.size();
	for(i=0; i<s; i++){
		const QString &t = q.shops.queryAt(i);
		insert(q, t);
	}
}

void QueryShopsTable::update(const Query &orig, const Query &modified)
{
	unsigned i, s = modified.shops.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.shops.queryAt(i);
		if(!orig.shops.has(mTag))
			insert(modified, mTag);
	}

	s = orig.shops.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.shops.queryAt(i);
		if(!modified.shops.has(oTag))
			del(orig, oTag);
	}
}

void QueryShopsTable::query(const Query &q, ShopNameSet &shops)
{
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, shop FROM QueryShops "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	selectQuery.exec();

	shops.clear();

	int shopNo = selectQuery.colIndex("shop");

	DBG("----- Query shops query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		shops.add(new QString(selectQuery.value(shopNo).toString()));
	}
	DBG("-----");
	selectQuery.finish();
}
