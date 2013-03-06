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

#include "butler_sqlite_shop.h"

namespace Sqlite {
	
ShopTable::ShopTable(Sql &_sql) :
	sql(_sql),
	insertQuery(sql),
	updateQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

ShopTable::~ShopTable()
{
}

void ShopTable::create()
{
	return sql.exec("CREATE TABLE Shops ("
			"name VARCHAR(64) NOT NULL PRIMARY KEY, "
			"store_name VARCHAR(256) NOT NULL, "
			"city VARCHAR(64) NOT NULL, "
			"address VARCHAR(256) NOT NULL, "
			"company VARCHAR(256) NOT NULL "
			")"
			);
}

void ShopTable::check(QStringList &tables)
{
	bool ret = true;

	ret = tables.contains("Shops");

	if(ret){
		QSqlRecord table = sql.record("Shops");
		if(		!table.contains("name") ||
				!table.contains("store_name") ||
				!table.contains("city") ||
				!table.contains("address") ||
				!table.contains("company")
		  ) {
			ret = false;
			LOG("Incompatible table Shops in the openend database.");
		}
	}

	return ret;
}

void ShopTable::insert(const Shop &s)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		ret = insertQuery.prepare("INSERT INTO Shops "
				"(name, store_name, city, "
				"address, company) "
				"VALUES(?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, s.name);
	insertQuery.bindValue(1, s.storeName);
	insertQuery.bindValue(2, s.city);
	insertQuery.bindValue(3, s.address);
	insertQuery.bindValue(4, s.company);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

void ShopTable::update(const Shop &orig, const Shop &modified)
{
	bool ret = true;

	if(!updateQuery.isPrepared())
		ret = updateQuery.prepare("UPDATE Shops SET "
				"name = ?, "
				"store_name = ?, "
				"city = ?, "
				"address = ?, "
				"company = ? "
				"WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.storeName);
	updateQuery.bindValue(2, modified.city);
	updateQuery.bindValue(3, modified.address);
	updateQuery.bindValue(4, modified.company);
	updateQuery.bindValue(5, orig.name);
	ret = ret && updateQuery.exec();
	updateQuery.finish();

	return ret;
}

void ShopTable::del(const Shop &s)
{
	bool ret = true;
	
	if(!deleteQuery.isPrepared())
		ret = deleteQuery.prepare(
				"DELETE FROM Shops WHERE "
				"name = ?");

	deleteQuery.bindValue(0, s.name);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

void ShopTable::query(ShopSet& ss)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT * FROM Shops");

	ret = ret && selectQuery.exec();

	if(ret){
		ss.clear();

		int nameNo = selectQuery.colIndex("name");
		int storeNameNo = selectQuery.colIndex("store_name");
		int cityNo = selectQuery.colIndex("city");
		int addressNo = selectQuery.colIndex("address");
		int companyNo = selectQuery.colIndex("company");

		DBG("----- Shop query result:");
		while(selectQuery.next()) {
			Shop *s = new Shop();
			s->name = selectQuery.value(nameNo).toString();
			s->storeName = selectQuery.value(storeNameNo).toString();
			s->city = selectQuery.value(cityNo).toString();
			s->address = selectQuery.value(addressNo).toString();
			s->company = selectQuery.value(companyNo).toString();

			ss.add(s);
		}
		DBG("-----");
	}

	return ret;
}

}


