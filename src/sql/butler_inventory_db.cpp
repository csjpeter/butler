/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#if 0
//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include "butler_inventory_db.h"

InventoryDb::InventoryDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("inventories"))
		sql.exec(
				"CREATE TABLE inventories ("
				"name TEXT, "
				"comment TEXT NOT NULL DEFAULT '',"
				"icon TEXT, "
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
				"" // keys
				"PRIMARY KEY (name)"
				")"
				);

	cols = sql.columns("inventories");
	if(		!cols.has("name") ||
			!cols.has("comment") ||
			!cols.has("icon") ||
			!cols.has("last_modified")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table inventories in the openend database.");
}

InventoryDb::~InventoryDb()
{
}

void InventoryDb::insert(const Inventory & inventory)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO inventories (name, comment) VALUES(?, ?)");
	sqlQuery.bindValue(0, inventory.name);
	sqlQuery.bindValue(1, inventory.comment);
	sqlQuery.exec();

	tr.commit();
}

void InventoryDb::update(const Inventory & orig, const Inventory & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* FIXME check if orig and modified are really different. Note: we need this check here
	 * because a inventory object is saved into multiple tables and some tables might require
	 * update some might not. We also need it because it is important to avoid unneccessary
	 * network traffic. */
	sqlQuery.prepare("UPDATE inventories SET name = ?, comment = ? WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.comment);
	sqlQuery.bindValue(2, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void InventoryDb::del(const Inventory & inventory)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM inventories WHERE name = ?");
	sqlQuery.bindValue(0, inventory.name);
	sqlQuery.exec();

	tr.commit();
}

void InventoryDb::query(InventorySet & inventories)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT name, comment FROM inventories");
	sqlQuery.exec();
	inventories.clear();
	int nameNo = sqlQuery.colIndex("name");
	int unitNo = sqlQuery.colIndex("comment");
	DBG("----- InventoryName query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Inventory *inventory = new Inventory();
		inventory->name = sqlQuery.text(nameNo);
		inventory->comment = sqlQuery.text(unitNo);
		inventories.add(inventory);
	}
	DBG("-----");

	tr.commit();
}
#endif
