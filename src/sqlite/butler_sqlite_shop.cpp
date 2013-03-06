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

void ShopTable::check(QStringList &tables)
{
	if(!tables.contains("Shops"))
		sql.exec("CREATE TABLE Shops ("
				"name VARCHAR(64) NOT NULL PRIMARY KEY, "
				"store_name VARCHAR(256) NOT NULL, "
				"city VARCHAR(64) NOT NULL, "
				"address VARCHAR(256) NOT NULL, "
				"company VARCHAR(256) NOT NULL "
				")"
			       );

	QSqlRecord table = sql.record("Shops");
	if(		!table.contains("name") ||
			!table.contains("store_name") ||
			!table.contains("city") ||
			!table.contains("address") ||
			!table.contains("company")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table Shops in the openend database.");
}

void ShopTable::insert(const Shop &s)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Shops "
				"(name, store_name, city, "
				"address, company) "
				"VALUES(?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, s.name);
	insertQuery.bindValue(1, s.storeName);
	insertQuery.bindValue(2, s.city);
	insertQuery.bindValue(3, s.address);
	insertQuery.bindValue(4, s.company);
	insertQuery.exec();
	insertQuery.finish();
}

void ShopTable::update(const Shop &orig, const Shop &modified)
{
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Shops SET "
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
	updateQuery.exec();
	updateQuery.finish();
}

void ShopTable::del(const Shop &s)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM Shops WHERE "
				"name = ?");

	deleteQuery.bindValue(0, s.name);
	deleteQuery.exec();
	deleteQuery.finish();
}

void ShopTable::query(ShopSet& ss)
{
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT * FROM Shops");

	selectQuery.exec();

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

}
