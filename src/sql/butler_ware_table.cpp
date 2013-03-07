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

#include "butler_ware_table.h"

WareTable::WareTable(SqlConnection &_sql) :
	sql(_sql),
	insertQuery(sql),
	updateQuery(sql),
	deleteQuery(sql),
	selectQuery(sql),
	selectAllQuery(sql)
{
}

WareTable::~WareTable()
{
}

void WareTable::check(QStringList &tables)
{
	if(!tables.contains("Wares"))
		sql.exec(
				  "CREATE TABLE Wares ("
				  "name VARCHAR(64) NOT NULL PRIMARY KEY, "
				  "unit VARCHAR(16) NOT NULL DEFAULT '' "
				  ")"
				  );

	QSqlRecord table = sql.record("Wares");
	if(		!table.contains("name") ||
			!table.contains("unit")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table Wares in the openend database.");
}

void WareTable::insert(const Ware &w)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Wares "
				"(name, unit) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, w.name);
	insertQuery.bindValue(1, w.unit);
	insertQuery.exec();
	insertQuery.finish();
}

void WareTable::update(const Ware &orig, const Ware &modified)
{
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Wares SET "
				"name = ?, "
				"unit = ? "
				"WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.unit);
	updateQuery.bindValue(2, orig.name);
	updateQuery.exec();
	updateQuery.finish();
}

void WareTable::del(const Ware &ware)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM Wares WHERE "
				"name = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.exec();
	deleteQuery.finish();
}

void WareTable::query(WareSet &wares)
{
	if(!selectAllQuery.isPrepared())
		selectAllQuery.prepare("SELECT name, unit FROM Wares");

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

	selectAllQuery.finish();
}
