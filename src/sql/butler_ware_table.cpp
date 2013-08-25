/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QVariant>

#include "butler_ware_table.h"

WareTable::WareTable(SqlConnection &_sql) :
	sql(_sql)
{
}

WareTable::~WareTable()
{
}

void WareTable::check(QStringList &tables)
{
	if(!tables.contains("wares"))
		sql.exec(
				  "CREATE TABLE wares ("
				  "name VARCHAR(64) NOT NULL PRIMARY KEY, "
				  "unit VARCHAR(16) NOT NULL DEFAULT '' "
				  ")"
				  );

	SqlColumns cols = sql.columns("wares");
	if(		!cols.has("name") ||
			!cols.has("unit")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table wares in the openend database.");
}

void WareTable::insert(const Ware &w)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO wares "
				"(name, unit) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, w.name);
	insertQuery.bindValue(1, w.unit);
	insertQuery.exec();
}

void WareTable::update(const Ware &orig, const Ware &modified)
{
	/* FIXME check if orig and modified are really different. Note: we need this check here
	 * because a ware object is saved into multiple tables and some tables might require
	 * update some might not. We also need it because it is important to avoid unneccessary
	 * network traffic. */
	SqlQuery updateQuery(sql);
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE wares SET "
				"name = ?, "
				"unit = ? "
				"WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.unit);
	updateQuery.bindValue(2, orig.name);
	updateQuery.exec();
}

void WareTable::del(const Ware &ware)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM wares WHERE "
				"name = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.exec();
}

void WareTable::query(WareSet &wares)
{
	SqlQuery selectAllQuery(sql);
	if(!selectAllQuery.isPrepared())
		selectAllQuery.prepare("SELECT name, unit FROM wares");

	selectAllQuery.exec();

	wares.clear();

	int nameNo = selectAllQuery.colIndex("name");
	int unitNo = selectAllQuery.colIndex("unit");

	DBG("----- WareName query result:");
	while (selectAllQuery.next()) {
		DBG("Next row");
		Ware *ware = new Ware();
		ware->name = selectAllQuery.value(nameNo).toString();
		ware->unit = selectAllQuery.value(unitNo).toString();
		wares.add(ware);
	}
	DBG("-----");
}
