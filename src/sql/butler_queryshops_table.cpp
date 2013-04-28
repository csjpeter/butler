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
	sql(_sql)
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

void QueryShopsTable::insert(const Query &q, const QString &partnerName)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO QueryShops "
				"(query_name, shop) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, partnerName);
	insertQuery.exec();
}

void QueryShopsTable::del(const Query &q, const QString &partnerName)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM QueryShops WHERE "
				"query_name = ? AND shop = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, partnerName);
	deleteQuery.exec();
}

void QueryShopsTable::insert(const Query &q)
{
	unsigned i, s = q.partners.size();
	for(i=0; i<s; i++){
		const QString &t = q.partners.queryAt(i);
		insert(q, t);
	}
}

void QueryShopsTable::update(const Query &orig, const Query &modified)
{
	unsigned i, s = modified.partners.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.partners.queryAt(i);
		if(!orig.partners.has(mTag))
			insert(modified, mTag);
	}

	s = orig.partners.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.partners.queryAt(i);
		if(!modified.partners.has(oTag))
			del(orig, oTag);
	}
}

void QueryShopsTable::query(const Query &q, ShopNameSet &partners)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, shop FROM QueryShops "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	selectQuery.exec();

	partners.clear();

	int partnerNo = selectQuery.colIndex("shop");

	DBG("----- Query partners query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		partners.add(new QString(selectQuery.value(partnerNo).toString()));
	}
	DBG("-----");
}
