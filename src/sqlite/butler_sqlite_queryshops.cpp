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

#include <butler_query.h>

#include "butler_sqlite_queryshops.h"

namespace Sqlite {

QueryShopsTable::QueryShopsTable(Sql &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

QueryShopsTable::~QueryShopsTable()
{
}

bool QueryShopsTable::create()
{
	bool ret = true;

	ret = ret && sql.exec("CREATE TABLE QueryShops ("
			"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
			"shop VARCHAR(64) NOT NULL REFERENCES Tags(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"UNIQUE (query_name, shop)"
			")"
			);
	ret = ret && sql.exec("CREATE INDEX QueryShopsQueryNameIndex "
			"ON QueryShops(query_name)");
	ret = ret && sql.exec("CREATE INDEX QueryShopsTagIndex "
			"ON QueryShops(shop)");

	return ret;
}

bool QueryShopsTable::check(QStringList &tables)
{
	bool ret;

	ret = tables.contains("QueryShops");

	if(ret){
		QSqlRecord table = sql.record("QueryShops");
		if(		!table.contains("query_name") ||
				!table.contains("shop")
		  ) {
			ret = false;
			CRITICAL("Incompatible table QueryShops "
					"in the openend database.");
		}
	}

	return ret;
}

bool QueryShopsTable::insert(const Query &q, const QString &shopName)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		ret = insertQuery.prepare("INSERT INTO QueryShops "
				"(query_name, shop) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, shopName);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool QueryShopsTable::del(const Query &q, const QString &shopName)
{
	bool ret = true;

	if(!deleteQuery.isPrepared())
		ret = deleteQuery.prepare(
				"DELETE FROM QueryShops WHERE "
				"query_name = ? AND shop = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, shopName);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool QueryShopsTable::insert(const Query &q)
{
	bool ret = true;

	unsigned i, s = q.shops.size();
	for(i=0; i<s; i++){
		const QString &t = q.shops.queryAt(i);
		ret = insert(q, t);
		if(!ret)
			break;
	}

	return ret;
}

bool QueryShopsTable::update(const Query &orig, const Query &modified)
{
	bool ret = true;

	unsigned i, s = modified.shops.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.shops.queryAt(i);
		if(!orig.shops.has(mTag)){
			ret = insert(modified, mTag);
			if(!ret)
				break;
			continue;
		}
	}

	s = orig.shops.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.shops.queryAt(i);
		if(!modified.shops.has(oTag)){
			ret = del(orig, oTag);
			if(!ret)
				break;
			continue;
		}
	}

	return ret;
}

bool QueryShopsTable::query(const Query &q, ShopNameSet &shops)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		ret = selectQuery.prepare("SELECT query_name, shop FROM QueryShops "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	ret = ret && selectQuery.exec();

	if(ret){
		shops.clear();

		int shopNo = selectQuery.colIndex("shop");

		DBG("----- Query shops query result:");
		while (selectQuery.next()) {
			DBG("Next row");
			shops.add(new QString(selectQuery.value(shopNo).toString()));
		}
		DBG("-----");
	}
	selectQuery.finish();

	return ret;
}

}

