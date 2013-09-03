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
	SqlTransaction tr(sql);

	SqlQuery insertQuery(sql);
	insertQuery.prepare("INSERT INTO partners "
			"(name, country, city, postal_code, address, company, store_name) "
			"VALUES(?, ?, ?, ?, ?, ?, ?)");
	insertQuery.bindValue(0, s.name);
	insertQuery.bindValue(1, s.country);
	insertQuery.bindValue(2, s.city);
	insertQuery.bindValue(3, s.postalCode);
	insertQuery.bindValue(4, s.address);
	insertQuery.bindValue(5, s.company);
	insertQuery.bindValue(6, s.storeName);
	insertQuery.exec();

	tr.commit();
}

void PartnerDb::update(const Partner & orig, const Partner & modified)
{
	SqlTransaction tr(sql);

	SqlQuery updateQuery(sql);
	updateQuery.prepare("UPDATE partners SET "
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

	tr.commit();
}

void PartnerDb::del(const Partner & s)
{
	SqlTransaction tr(sql);

	SqlQuery deleteQuery(sql);
	deleteQuery.prepare("DELETE FROM partners WHERE name = ?");
	deleteQuery.bindValue(0, s.name);
	deleteQuery.exec();

	tr.commit();
}

void PartnerDb::query(PartnerSet & ss)
{
	SqlTransaction tr(sql);

	SqlQuery selectQuery(sql);
	selectQuery.prepare("SELECT "
			"name, country, city, postal_code, address, company, store_name "
			"FROM partners");
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
		s->name = selectQuery.text(nameNo);
		s->country = selectQuery.text(countryNo);
		s->city = selectQuery.text(cityNo);
		s->postalCode = selectQuery.text(postalCodeNo);
		s->address = selectQuery.text(addressNo);
		s->company = selectQuery.text(companyNo);
		s->storeName = selectQuery.text(storeNameNo);
		ss.add(s);
	}
	DBG("-----");

	tr.commit();
}
