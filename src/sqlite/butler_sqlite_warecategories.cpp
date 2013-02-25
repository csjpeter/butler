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

#include "butler_sqlite_warecategories.h"

namespace Sqlite {

WareCategoriesTable::WareCategoriesTable(Sql &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

WareCategoriesTable::~WareCategoriesTable()
{
}

bool WareCategoriesTable::create()
{
	bool ret = true;

	ret = ret && sql.exec("CREATE TABLE WareCategories ("
			"name VARCHAR(64) NOT NULL REFERENCES Wares(name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
			"category VARCHAR(32) NOT NULL, "
			"UNIQUE (name, category) "
			")"
			);
	ret = ret && sql.exec("CREATE INDEX WareCategoriesNameIndex "
			"ON WareCategories(name)");
	ret = ret && sql.exec("CREATE INDEX WareCategoriesCategoryIndex "
			"ON WareCategories(category)");

	return ret;
}

bool WareCategoriesTable::check(QStringList &tables)
{
	bool ret = true;

	ret = tables.contains("WareCategories");

	if(ret){
		QSqlRecord table = sql.record("WareCategories");
		if(		!table.contains("name") ||
				!table.contains("category")
		  ) {
			ret = false;
			LOG("Incompatible table WareCategories in the openend database.");
		}
	}

	return ret;
}

bool WareCategoriesTable::insert(const Ware &ware, const QString &category)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO WareCategories "
				"(name, category) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, category);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool WareCategoriesTable::del(const Ware &ware, const QString &category)
{
	bool ret = true;

	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM WareCategories WHERE "
				"name = ? AND "
				"category = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.bindValue(1, category);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool WareCategoriesTable::insert(const Ware &ware)
{
	bool ret = true;

	unsigned i, s = ware.categories.size();
	for(i=0; i<s && ret; i++){
		const QString &category = ware.categories.queryAt(i);
		ret = insert(ware, category);
	}

	return ret;
}

bool WareCategoriesTable::update(const Ware &orig, const Ware &modified)
{
	bool ret = true;

	unsigned i, s;
	
	s = modified.categories.size();
	for(i=0; i<s && ret; i++){
		const QString &category = modified.categories.queryAt(i);
		if(!orig.categories.has(category))
			ret = insert(modified, category);
	}
	
	s = orig.categories.size();
	for(i=0; i<s && ret; i++){
		const QString &category = orig.categories.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If Wares table is updated already.) */
		if(!modified.categories.has(category))
			ret = del(modified, category);
	}

	return ret;
}

bool WareCategoriesTable::query(const Ware &ware, CategoryNameSet &categories)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT category FROM WareCategories "
				"WHERE name = ?");

	selectQuery.bindValue(0, ware.name);
	ret = ret && selectQuery.exec();

	if(ret){
		int categoryNo = selectQuery.colIndex("category");

		DBG("----- Ware categories query result:");
		while (selectQuery.next()) {
			DBG("Next row");
			categories.add(new QString(selectQuery.value(categoryNo).toString()));
		}
		DBG("-----");
	}

	selectQuery.finish();

	return ret;
}

}
