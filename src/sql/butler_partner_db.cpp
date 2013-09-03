/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_partner_db.h"

PartnerDb::PartnerDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("partners"))
		sql.exec("CREATE TABLE partners ("
				"name VARCHAR(64) NOT NULL PRIMARY KEY, "
				"country VARCHAR(64) NOT NULL, "
				"city VARCHAR(64) NOT NULL, "
				"postal_code VARCHAR(32) NOT NULL, "
				"address VARCHAR(256) NOT NULL, "
				"company VARCHAR(64) NOT NULL REFERENCES company(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"store_name VARCHAR(256) NOT NULL "
				")"
			       );
	cols = sql.columns("partners");
	if(		!cols.has("name") ||
			!cols.has("country") ||
			!cols.has("city") ||
			!cols.has("postal_code") ||
			!cols.has("address") ||
			!cols.has("company") ||
			!cols.has("store_name")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table partners in the openend database.");
}

PartnerDb::~PartnerDb()
{
}

void PartnerDb::insert(const Partner & s)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO partners "
			"(name, country, city, postal_code, address, company, store_name) "
			"VALUES(?, ?, ?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, s.name);
	sqlQuery.bindValue(1, s.country);
	sqlQuery.bindValue(2, s.city);
	sqlQuery.bindValue(3, s.postalCode);
	sqlQuery.bindValue(4, s.address);
	sqlQuery.bindValue(5, s.company);
	sqlQuery.bindValue(6, s.storeName);
	sqlQuery.exec();

	tr.commit();
}

void PartnerDb::update(const Partner & orig, const Partner & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE partners SET "
			"name = ?, "
			"country = ?, "
			"city = ?, "
			"postal_code = ?, "
			"address = ?, "
			"company = ?, "
			"store_name = ? "
			"WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.country);
	sqlQuery.bindValue(2, modified.city);
	sqlQuery.bindValue(3, modified.postalCode);
	sqlQuery.bindValue(4, modified.address);
	sqlQuery.bindValue(5, modified.company);
	sqlQuery.bindValue(6, modified.storeName);
	sqlQuery.bindValue(7, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void PartnerDb::del(const Partner & s)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM partners WHERE name = ?");
	sqlQuery.bindValue(0, s.name);
	sqlQuery.exec();

	tr.commit();
}

void PartnerDb::query(PartnerSet & ss)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT "
			"name, country, city, postal_code, address, company, store_name "
			"FROM partners");
	sqlQuery.exec();
	ss.clear();
	int nameNo = sqlQuery.colIndex("name");
	int countryNo = sqlQuery.colIndex("country");
	int cityNo = sqlQuery.colIndex("city");
	int postalCodeNo = sqlQuery.colIndex("postal_code");
	int addressNo = sqlQuery.colIndex("address");
	int companyNo = sqlQuery.colIndex("company");
	int storeNameNo = sqlQuery.colIndex("store_name");
	DBG("----- Partner query result:");
	while(sqlQuery.next()) {
		Partner *s = new Partner();
		s->name = sqlQuery.text(nameNo);
		s->country = sqlQuery.text(countryNo);
		s->city = sqlQuery.text(cityNo);
		s->postalCode = sqlQuery.text(postalCodeNo);
		s->address = sqlQuery.text(addressNo);
		s->company = sqlQuery.text(companyNo);
		s->storeName = sqlQuery.text(storeNameNo);
		ss.add(s);
	}
	DBG("-----");

	tr.commit();
}
