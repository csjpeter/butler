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

#include "butler_partner_table.h"

PartnerTable::PartnerTable(SqlConnection &_sql) :
	sql(_sql)
{
}

PartnerTable::~PartnerTable()
{
}

void PartnerTable::check(QStringList &tables)
{
	if(!tables.contains("Partners"))
		sql.exec("CREATE TABLE Partners ("
				"name VARCHAR(64) NOT NULL PRIMARY KEY, "
				"country VARCHAR(64) NOT NULL, "
				"city VARCHAR(64) NOT NULL, "
				"postal_code VARCHAR(32) NOT NULL, "
				"address VARCHAR(256) NOT NULL, "
				"company VARCHAR(64) NOT NULL REFERENCES Company(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"store_name VARCHAR(256) NOT NULL "
				")"
			       );

	QSqlRecord table = sql.record("Partners");
	if(		!table.contains("name") ||
			!table.contains("country") ||
			!table.contains("city") ||
			!table.contains("postal_code") ||
			!table.contains("address") ||
			!table.contains("company") ||
			!table.contains("store_name")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table Partners in the openend database.");
}

void PartnerTable::insert(const Partner &s)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Partners "
				"(name, store_name, country, city, postal_code, "
				"address, company) "
				"VALUES(?, ?, ?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, s.name);
	insertQuery.bindValue(1, s.country);
	insertQuery.bindValue(2, s.city);
	insertQuery.bindValue(3, s.postalCode);
	insertQuery.bindValue(4, s.address);
	insertQuery.bindValue(5, s.company);
	insertQuery.bindValue(6, s.storeName);
	insertQuery.exec();
}

void PartnerTable::update(const Partner &orig, const Partner &modified)
{
	SqlQuery updateQuery(sql);
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Partners SET "
				"name = ?, "
				"country = ?, "
				"city = ?, "
				"postal_code = ?, "
				"address = ?, "
				"company = ?, "
				"store_name = ? "
				"WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.country);
	updateQuery.bindValue(2, modified.city);
	updateQuery.bindValue(3, modified.postalCode);
	updateQuery.bindValue(4, modified.address);
	updateQuery.bindValue(5, modified.company);
	updateQuery.bindValue(6, modified.storeName);
	updateQuery.bindValue(7, orig.name);
	updateQuery.exec();
}

void PartnerTable::del(const Partner &s)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM Partners WHERE "
				"name = ?");

	deleteQuery.bindValue(0, s.name);
	deleteQuery.exec();
}

void PartnerTable::query(PartnerSet& ss)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT * FROM Partners");

	selectQuery.exec();

	ss.clear();

	int nameNo = selectQuery.colIndex("name");
	int countryNo = selectQuery.colIndex("country");
	int cityNo = selectQuery.colIndex("city");
	int postalCodeNo = selectQuery.colIndex("postal_code");
	int addressNo = selectQuery.colIndex("address");
	int companyNo = selectQuery.colIndex("company");
	int storeNameNo = selectQuery.colIndex("store_name");

	DBG("----- Partner query result:");
	while(selectQuery.next()) {
		Partner *s = new Partner();
		s->name = selectQuery.value(nameNo).toString();
		s->country = selectQuery.value(countryNo).toString();
		s->city = selectQuery.value(cityNo).toString();
		s->postalCode = selectQuery.value(postalCodeNo).toString();
		s->address = selectQuery.value(addressNo).toString();
		s->company = selectQuery.value(companyNo).toString();
		s->storeName = selectQuery.value(storeNameNo).toString();

		ss.add(s);
	}
	DBG("-----");
}
