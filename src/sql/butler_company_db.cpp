/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#if 0
#include "butler_company_db.h"

CompanyDb::CompanyDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("companies"))
		sql.exec("CREATE TABLE companies ("
				"name TEXT, "
				"country TEXT NOT NULL, "
				"city TEXT NOT NULL, "
				"postal_code TEXT NOT NULL, "
				"address TEXT NOT NULL, "
				"tax_id TEXT NOT NULL, "
				"icon TEXT, "
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
				"" // keys
				"PRIMARY KEY (name)"
				")"
			       );
	cols = sql.columns("companies");
	if(		!cols.has("name") ||
			!cols.has("country") ||
			!cols.has("city") ||
			!cols.has("postal_code") ||
			!cols.has("address") ||
			!cols.has("tax_id") ||
			!cols.has("icon") ||
			!cols.has("last_modified")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table companies in the openend database.");
}

CompanyDb::~CompanyDb()
{
}

void CompanyDb::insert(const Company & s)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO companies "
			"(name, country, city, postal_code, "
			"address, tax_id) "
			"VALUES(?, ?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, s.name);
	sqlQuery.bindValue(1, s.country);
	sqlQuery.bindValue(2, s.city);
	sqlQuery.bindValue(3, s.postalCode);
	sqlQuery.bindValue(4, s.address);
	sqlQuery.bindValue(5, s.taxId);
	sqlQuery.exec();

	tr.commit();
}

void CompanyDb::update(const Company & orig, const Company & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE companies SET "
			"name = ?, "
			"country = ?, "
			"city = ?, "
			"postal_code = ?, "
			"address = ?, "
			"tax_id = ? "
			"WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.country);
	sqlQuery.bindValue(2, modified.city);
	sqlQuery.bindValue(3, modified.postalCode);
	sqlQuery.bindValue(4, modified.address);
	sqlQuery.bindValue(5, modified.taxId);
	sqlQuery.bindValue(6, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void CompanyDb::del(const Company & s)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM companies WHERE name = ?");
	sqlQuery.bindValue(0, s.name);
	sqlQuery.exec();

	tr.commit();
}

void CompanyDb::query(CompanySet & ss)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT name, country, city, postal_code, address, tax_id "
			"FROM companies");
	sqlQuery.exec();
	ss.clear();
	int nameNo = sqlQuery.colIndex("name");
	int countryNo = sqlQuery.colIndex("country");
	int cityNo = sqlQuery.colIndex("city");
	int postalCodeNo = sqlQuery.colIndex("postal_code");
	int addressNo = sqlQuery.colIndex("address");
	int taxIdNo = sqlQuery.colIndex("tax_id");
	DBG("----- companies query result:");
	while(sqlQuery.next()) {
		Company *s = new Company();
		s->name = sqlQuery.text(nameNo);
		s->country = sqlQuery.text(countryNo);
		s->city = sqlQuery.text(cityNo);
		s->postalCode = sqlQuery.text(postalCodeNo);
		s->address = sqlQuery.text(addressNo);
		s->taxId = sqlQuery.text(taxIdNo);

		ss.add(s);
	}
	DBG("-----");

	tr.commit();
}
#endif
