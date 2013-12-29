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
				"name TEXT NOT NULL PRIMARY KEY, "
				"country TEXT NOT NULL, "
				"city TEXT NOT NULL, "
				"postalCode TEXT NOT NULL, "
				"address TEXT NOT NULL, "
				"company TEXT NOT NULL REFERENCES companies(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"storeName TEXT NOT NULL, "
				"lastModified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP "
				")"
			       );
	cols = sql.columns("partners");
	if(		!cols.has("name") ||
			!cols.has("country") ||
			!cols.has("city") ||
			( !cols.has("postalCode") && !cols.has("postalcode") ) ||
			!cols.has("address") ||
			!cols.has("company") ||
			( !cols.has("storeName") && !cols.has("storename") ) ||
			( !cols.has("lastModified") && !cols.has("lastmodified") )
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
			"(name, country, city, postalCode, address, company, storeName) "
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
			"postalCode = ?, "
			"address = ?, "
			"company = ?, "
			"storeName = ? "
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
			"name, country, city, postalCode, address, company, storeName "
			"FROM partners");
	sqlQuery.exec();
	ss.clear();
	int nameNo = sqlQuery.colIndex("name");
	int countryNo = sqlQuery.colIndex("country");
	int cityNo = sqlQuery.colIndex("city");
	int postalCodeNo = sqlQuery.colIndex("postalCode");
	int addressNo = sqlQuery.colIndex("address");
	int companyNo = sqlQuery.colIndex("company");
	int storeNameNo = sqlQuery.colIndex("storeName");
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
