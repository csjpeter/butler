/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_ware_db.h"

WareDb::WareDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("wares"))
		sql.exec(
				  "CREATE TABLE wares ("
				  "name VARCHAR(64) NOT NULL PRIMARY KEY, "
				  "unit VARCHAR(16) NOT NULL DEFAULT '' "
				  ")"
				  );

	cols = sql.columns("wares");
	if(		!cols.has("name") ||
			!cols.has("unit")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table wares in the openend database.");

	if(!tables.has("ware_tags")){
		sql.exec("CREATE TABLE ware_tags ("
				"name VARCHAR(64) NOT NULL REFERENCES wares(name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
				"tag VARCHAR(64) NOT NULL REFERENCES tags(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"UNIQUE (name, tag) "
				")"
				);
		sql.exec("CREATE INDEX ware_tags_name_index ON ware_tags(name)");
		sql.exec("CREATE INDEX ware_tags_tag_index ON ware_tags(tag)");
	}

	cols = sql.columns("ware_tags");
	if(		!cols.has("name") ||
			!cols.has("tag")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table ware_tags in the openend database.");

	if(!tables.has("ware_categories")){
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

	cols = sql.columns("ware_categories");
	if(		!cols.has("name") ||
			!cols.has("category")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table ware_categories in the openend database.");
}

WareDb::~WareDb()
{
}

void WareDb::insert(const Ware & ware)
{
	SqlTransaction tr(sql);

	SqlQuery insertQuery(sql);
	insertQuery.prepare("INSERT INTO wares (name, unit) VALUES(?, ?)");
	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, ware.unit);
	insertQuery.exec();

	unsigned i, s = ware.tags.size();
	for(i=0; i<s; i++){
		const Text & tag = ware.tags.queryAt(i);
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO ware_tags (name, tag) VALUES(?, ?)");
		insertQuery.bindValue(0, ware.name);
		insertQuery.bindValue(1, tag);
		insertQuery.exec();
	}

	s = ware.categories.size();
	for(i=0; i<s; i++){
		const Text & category = ware.categories.queryAt(i);
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO ware_categories (name, category) VALUES(?, ?)");
		insertQuery.bindValue(0, ware.name);
		insertQuery.bindValue(1, category);
		insertQuery.exec();
	}

	tr.commit();
}

void WareDb::update(const Ware & orig, const Ware & modified)
{
	SqlTransaction tr(sql);

	/* FIXME check if orig and modified are really different. Note: we need this check here
	 * because a ware object is saved into multiple tables and some tables might require
	 * update some might not. We also need it because it is important to avoid unneccessary
	 * network traffic. */
	SqlQuery updateQuery(sql);
	updateQuery.prepare("UPDATE wares SET name = ?, unit = ? WHERE name = ?");
	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.unit);
	updateQuery.bindValue(2, orig.name);
	updateQuery.exec();

	unsigned i, s;
	s = modified.tags.size();
	for(i=0; i<s; i++){
		const Text & tag = modified.tags.queryAt(i);
		if(orig.tags.has(tag))
			continue;
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO ware_tags (name, tag) VALUES(?, ?)");
		insertQuery.bindValue(0, modified.name);
		insertQuery.bindValue(1, tag);
		insertQuery.exec();
	}
	s = orig.tags.size();
	for(i=0; i<s; i++){
		const Text & tag = orig.tags.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If wares table is updated already.) */
		if(!modified.tags.has(tag)){
			SqlQuery deleteQuery(sql);
			deleteQuery.prepare("DELETE FROM ware_tags WHERE name = ? AND tag = ?");
			deleteQuery.bindValue(0, modified.name);
			deleteQuery.bindValue(1, tag);
			deleteQuery.exec();
		}
	}

	s = modified.categories.size();
	for(i=0; i<s; i++){
		const Text & category = modified.categories.queryAt(i);
		if(orig.categories.has(category))
			continue;
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO ware_categories "
				"(name, category) "
				"VALUES(?, ?)");
		insertQuery.bindValue(0, modified.name);
		insertQuery.bindValue(1, category);
		insertQuery.exec();
	}
	s = orig.categories.size();
	for(i=0; i<s; i++){
		const Text & category = orig.categories.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If Wares table is updated already.) */
		if(!modified.categories.has(category)){
			SqlQuery deleteQuery(sql);
			deleteQuery.prepare("DELETE FROM ware_categories WHERE "
						"name = ? AND category = ?");
			deleteQuery.bindValue(0, modified.name);
			deleteQuery.bindValue(1, category);
			deleteQuery.exec();
		}
	}

	tr.commit();
}

void WareDb::del(const Ware & ware)
{
	SqlTransaction tr(sql);

	SqlQuery deleteQuery(sql);
	deleteQuery.prepare("DELETE FROM wares WHERE name = ?");
	deleteQuery.bindValue(0, ware.name);
	deleteQuery.exec();

	tr.commit();
}

void WareDb::query(WareSet & wares)
{
	SqlTransaction tr(sql);

	SqlQuery selectAllQuery(sql);
	selectAllQuery.prepare("SELECT name, unit FROM wares");
	selectAllQuery.exec();
	wares.clear();
	int nameNo = selectAllQuery.colIndex("name");
	int unitNo = selectAllQuery.colIndex("unit");
	DBG("----- WareName query result:");
	while (selectAllQuery.next()) {
		DBG("Next row");
		Ware *ware = new Ware();
		ware->name = selectAllQuery.text(nameNo);
		ware->unit = selectAllQuery.text(unitNo);
		wares.add(ware);
	}
	DBG("-----");

	SqlQuery selectQuery(sql);
	selectQuery.exec("SELECT name, tag FROM ware_tags");
	nameNo = selectQuery.colIndex("name");
	int tagNo = selectQuery.colIndex("tag");
	DBG("----- Ware tags query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		Text name(selectQuery.text(nameNo));
		if(wares.has(name))
			wares.query(name).tags.add(new Text(selectQuery.text(tagNo)));
	}
	DBG("-----");

	SqlQuery selectCategoryQuery(sql);
	selectCategoryQuery.exec("SELECT name, category FROM ware_categories");

	nameNo = selectCategoryQuery.colIndex("name");
	int categoryNo = selectCategoryQuery.colIndex("category");

	DBG("----- Ware categories query result:");
	while (selectCategoryQuery.next()) {
		DBG("Next row");
		Text name(selectCategoryQuery.text(nameNo));
		if(wares.has(name))
			wares.query(name).categories.add(
					new Text(selectCategoryQuery.text(categoryNo)));
	}
	DBG("-----");

	tr.commit();
}
