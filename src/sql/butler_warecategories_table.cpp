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

#include "butler_warecategories_table.h"

WareCategoriesTable::WareCategoriesTable(SqlConnection &_sql) :
	sql(_sql)
{
}

WareCategoriesTable::~WareCategoriesTable()
{
}


void WareCategoriesTable::check(QStringList &tables)
{
	if(!tables.contains("ware_categories")){
		sql.exec("CREATE TABLE ware_categories ("
				  "name VARCHAR(64) NOT NULL REFERENCES wares(name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "category VARCHAR(32) NOT NULL, "
				  "UNIQUE (name, category) "
				  ")"
			    );
		sql.exec("CREATE INDEX ware_categories_name_index "
				"ON ware_categories(name)");
		sql.exec("CREATE INDEX ware_categories_category_index "
				"ON ware_categories(category)");
	}

	QSqlRecord table = sql.record("ware_categories");
	if(		!table.contains("name") ||
			!table.contains("category")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table ware_categories in the openend database.");
}

void WareCategoriesTable::insert(const Ware &ware, const QString &category)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO ware_categories "
				"(name, category) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, category);
	insertQuery.exec();
}

void WareCategoriesTable::del(const Ware &ware, const QString &category)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM ware_categories WHERE "
				"name = ? AND "
				"category = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.bindValue(1, category);
	deleteQuery.exec();
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
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT category FROM ware_categories "
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
}
