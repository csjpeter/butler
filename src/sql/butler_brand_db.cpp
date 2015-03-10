/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#if 0
//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include "butler_brand_db.h"

BrandDb::BrandDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("brands"))
		sql.exec(
				"CREATE TABLE brands ("
				"name TEXT, "
				"company TEXT NOT NULL, "
				"icon TEXT, "
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
				"" // keys
				"PRIMARY KEY (name), "
				"FOREIGN KEY (company) REFERENCES companies(name) "
				"		ON DELETE RESTRICT ON UPDATE CASCADE "
				")"
				);

	cols = sql.columns("brands");
	if(		!cols.has("name") ||
			!cols.has("company") ||
			!cols.has("icon") ||
			!cols.has("last_modified")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table brands in the openend database.");
}

BrandDb::~BrandDb()
{
}

void BrandDb::insert(const Brand & brand)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO brands (name, company) VALUES(?, ?)");
	sqlQuery.bindValue(0, brand.name);
	sqlQuery.bindValue(1, brand.company);
	sqlQuery.exec();

	tr.commit();
}

void BrandDb::update(const Brand & orig, const Brand & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* FIXME check if orig and modified are really different. Note: we need this check here
	 * because a brand object is saved into multiple tables and some tables might require
	 * update some might not. We also need it because it is important to avoid unneccessary
	 * network traffic. */
	sqlQuery.prepare("UPDATE brands SET name = ?, company = ? WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.company);
	sqlQuery.bindValue(2, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void BrandDb::del(const Brand & brand)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM brands WHERE name = ?");
	sqlQuery.bindValue(0, brand.name);
	sqlQuery.exec();

	tr.commit();
}

void BrandDb::query(BrandSet & brands)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT name, company FROM brands");
	sqlQuery.exec();
	brands.clear();
	int nameNo = sqlQuery.colIndex("name");
	int unitNo = sqlQuery.colIndex("company");
	DBG("----- BrandName query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Brand *brand = new Brand();
		brand->name = sqlQuery.text(nameNo);
		brand->company = sqlQuery.text(unitNo);
		brands.add(brand);
	}
	DBG("-----");

	tr.commit();
}
#endif
