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

#include "butler_company_table.h"

CompanyTable::CompanyTable(SqlConnection &_sql) :
	sql(_sql)
{
}

CompanyTable::~CompanyTable()
{
}

void CompanyTable::check(QStringList &tables)
{
	if(!tables.contains("Company"))
		sql.exec("CREATE TABLE Company ("
				"name VARCHAR(64) NOT NULL PRIMARY KEY, "
				"country VARCHAR(64) NOT NULL, "
				"city VARCHAR(64) NOT NULL, "
				"postal_code VARCHAR(32) NOT NULL, "
				"address VARCHAR(256) NOT NULL, "
				"tax_id VARCHAR(32) NOT NULL "
				")"
			       );

	QSqlRecord table = sql.record("Company");
	if(		!table.contains("name") ||
			!table.contains("country") ||
			!table.contains("city") ||
			!table.contains("postal_code") ||
			!table.contains("address") ||
			!table.contains("tax_id")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table Company in the openend database.");
}

void CompanyTable::insert(const Company &s)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Company "
				"(name, country, city, postal_code, "
				"address, tax_id) "
				"VALUES(?, ?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, s.name);
	insertQuery.bindValue(1, s.country);
	insertQuery.bindValue(2, s.city);
	insertQuery.bindValue(3, s.postalCode);
	insertQuery.bindValue(4, s.address);
	insertQuery.bindValue(5, s.taxId);
	insertQuery.exec();
}

void CompanyTable::update(const Company &orig, const Company &modified)
{
	SqlQuery updateQuery(sql);
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Company SET "
				"name = ?, "
				"country = ?, "
				"city = ?, "
				"postal_code = ?, "
				"address = ?, "
				"tax_id = ? "
				"WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.country);
	updateQuery.bindValue(2, modified.city);
	updateQuery.bindValue(3, modified.postalCode);
	updateQuery.bindValue(4, modified.address);
	updateQuery.bindValue(5, modified.taxId);
	updateQuery.bindValue(6, orig.name);
	updateQuery.exec();
}

void CompanyTable::del(const Company &s)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM Company WHERE "
				"name = ?");

	deleteQuery.bindValue(0, s.name);
	deleteQuery.exec();
}

void CompanyTable::query(CompanySet& ss)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT * FROM Company");

	selectQuery.exec();

	ss.clear();

	int nameNo = selectQuery.colIndex("name");
	int countryNo = selectQuery.colIndex("country");
	int cityNo = selectQuery.colIndex("city");
	int postalCodeNo = selectQuery.colIndex("postal_code");
	int addressNo = selectQuery.colIndex("address");
	int taxIdNo = selectQuery.colIndex("tax_id");

	DBG("----- Company query result:");
	while(selectQuery.next()) {
		Company *s = new Company();
		s->name = selectQuery.value(nameNo).toString();
		s->country = selectQuery.value(countryNo).toString();
		s->city = selectQuery.value(cityNo).toString();
		s->postalCode = selectQuery.value(postalCodeNo).toString();
		s->address = selectQuery.value(addressNo).toString();
		s->taxId = selectQuery.value(taxIdNo).toString();

		ss.add(s);
	}
	DBG("-----");
}
