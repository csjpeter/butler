/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_company_db.h"

CompanyDb::CompanyDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("company"))
		sql.exec("CREATE TABLE company ("
				"name VARCHAR(64) NOT NULL PRIMARY KEY, "
				"country VARCHAR(64) NOT NULL, "
				"city VARCHAR(64) NOT NULL, "
				"postal_code VARCHAR(32) NOT NULL, "
				"address VARCHAR(256) NOT NULL, "
				"tax_id VARCHAR(32) NOT NULL "
				")"
			       );
	cols = sql.columns("company");
	if(		!cols.has("name") ||
			!cols.has("country") ||
			!cols.has("city") ||
			!cols.has("postal_code") ||
			!cols.has("address") ||
			!cols.has("tax_id")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table company in the openend database.");
}

CompanyDb::~CompanyDb()
{
}

void CompanyDb::insert(const Company & s)
{
	SqlTransaction tr(sql);

	SqlQuery insertQuery(sql);
	insertQuery.prepare("INSERT INTO company "
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

	tr.commit();
}

void CompanyDb::update(const Company & orig, const Company & modified)
{
	SqlTransaction tr(sql);

	SqlQuery updateQuery(sql);
	updateQuery.prepare("UPDATE company SET "
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

	tr.commit();
}

void CompanyDb::del(const Company & s)
{
	SqlTransaction tr(sql);

	SqlQuery deleteQuery(sql);
	deleteQuery.prepare("DELETE FROM company WHERE name = ?");
	deleteQuery.bindValue(0, s.name);
	deleteQuery.exec();

	tr.commit();
}

void CompanyDb::query(CompanySet & ss)
{
	SqlTransaction tr(sql);

	SqlQuery selectQuery(sql);
	selectQuery.prepare("SELECT name, country, city, postal_code, address, tax_id "
			"FROM company");
	selectQuery.exec();
	ss.clear();
	int nameNo = selectQuery.colIndex("name");
	int countryNo = selectQuery.colIndex("country");
	int cityNo = selectQuery.colIndex("city");
	int postalCodeNo = selectQuery.colIndex("postal_code");
	int addressNo = selectQuery.colIndex("address");
	int taxIdNo = selectQuery.colIndex("tax_id");
	DBG("----- company query result:");
	while(selectQuery.next()) {
		Company *s = new Company();
		s->name = selectQuery.text(nameNo);
		s->country = selectQuery.text(countryNo);
		s->city = selectQuery.text(cityNo);
		s->postalCode = selectQuery.text(postalCodeNo);
		s->address = selectQuery.text(addressNo);
		s->taxId = selectQuery.text(taxIdNo);

		ss.add(s);
	}
	DBG("-----");

	tr.commit();
}
