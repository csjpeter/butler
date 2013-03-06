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

WareCategoriesTable::WareCategoriesTable(SqlConnection &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

WareCategoriesTable::~WareCategoriesTable()
{
}


void WareCategoriesTable::check(QStringList &tables)
{
	if(!tables.contains("WareCategories")){
		sql.exec("CREATE TABLE WareCategories ("
				  "name VARCHAR(64) NOT NULL REFERENCES Wares(name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "category VARCHAR(32) NOT NULL, "
				  "UNIQUE (name, category) "
				  ")"
			    );
		sql.exec("CREATE INDEX WareCategoriesNameIndex "
				"ON WareCategories(name)");
		sql.exec("CREATE INDEX WareCategoriesCategoryIndex "
				"ON WareCategories(category)");
	}

	QSqlRecord table = sql.record("WareCategories");
	if(		!table.contains("name") ||
			!table.contains("category")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table WareCategories in the openend database.");
}

void WareCategoriesTable::insert(const Ware &ware, const QString &category)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO WareCategories "
				"(name, category) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, category);
	insertQuery.exec();
	insertQuery.finish();
}

void WareCategoriesTable::del(const Ware &ware, const QString &category)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM WareCategories WHERE "
				"name = ? AND "
				"category = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.bindValue(1, category);
	deleteQuery.exec();
	deleteQuery.finish();
}

void WareCategoriesTable::insert(const Ware &ware)
{
	unsigned i, s = ware.categories.size();
	for(i=0; i<s; i++){
		const QString &category = ware.categories.queryAt(i);
		insert(ware, category);
	}
}

void WareCategoriesTable::update(const Ware &orig, const Ware &modified)
{
	unsigned i, s;
	
	s = modified.categories.size();
	for(i=0; i<s; i++){
		const QString &category = modified.categories.queryAt(i);
		if(!orig.categories.has(category))
			insert(modified, category);
	}
	
	s = orig.categories.size();
	for(i=0; i<s; i++){
		const QString &category = orig.categories.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If Wares table is updated already.) */
		if(!modified.categories.has(category))
			del(modified, category);
	}
}

void WareCategoriesTable::query(const Ware &ware, CategoryNameSet &categories)
{
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT category FROM WareCategories "
				"WHERE name = ?");

	selectQuery.bindValue(0, ware.name);
	selectQuery.exec();

	int categoryNo = selectQuery.colIndex("category");

	DBG("----- Ware categories query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		categories.add(new QString(selectQuery.value(categoryNo).toString()));
	}
	DBG("-----");

	selectQuery.finish();
}
