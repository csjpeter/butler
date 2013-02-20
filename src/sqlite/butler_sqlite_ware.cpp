/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

#include "butler_sqlite_ware.h"

namespace Sqlite {
	
WareTable::WareTable(Sql &_sql) :
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

bool WareTable::create()
{
	return sql.exec("CREATE TABLE Wares ("
			"name VARCHAR(64) NOT NULL PRIMARY KEY, "
			"unit VARCHAR(16) NOT NULL DEFAULT '' "
			")"
			);
}

bool WareTable::check(QStringList &tables)
{
	bool ret = true;

	ret = tables.contains("Wares");

	if(ret){
		QSqlRecord table = sql.record("Wares");
		if(		!table.contains("name") ||
				!table.contains("unit")
		  ) {
			ret = false;
			CRITICAL("Incompatible table Wares "
					"in the openend database.");
		}
	}

	return ret;
}

bool WareTable::insert(const Ware &w)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		ret = insertQuery.prepare("INSERT INTO Wares "
				"(name, unit) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, w.name);
	insertQuery.bindValue(1, w.unit);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool WareTable::update(const Ware &orig, const Ware &modified)
{
	bool ret = true;

	if(!updateQuery.isPrepared())
		ret = updateQuery.prepare("UPDATE Wares SET "
				"name = ?, "
				"unit = ? "
				"WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.unit);
	updateQuery.bindValue(2, orig.name);
	ret = ret && updateQuery.exec();
	updateQuery.finish();

	return ret;
}

bool WareTable::del(const Ware &ware)
{
	bool ret = true;
	
	if(!deleteQuery.isPrepared())
		ret = deleteQuery.prepare(
				"DELETE FROM Wares WHERE "
				"name = ?");

	deleteQuery.bindValue(0, ware.name);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool WareTable::query(WareSet &wares)
{
	bool ret = true;

	if(!selectAllQuery.isPrepared())
		selectAllQuery.prepare("SELECT name, unit FROM Wares");

	ret = ret && selectAllQuery.exec();

	if(ret){
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
	selectAllQuery.finish();

	return ret;
}

}

