/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include "butler_ware_db.h"

WareDb::WareDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("wares"))
		sql.exec(
				"CREATE TABLE wares ("
				"name TEXT NOT NULL PRIMARY KEY, "
				"unit TEXT NOT NULL DEFAULT '', "
				"icon TEXT, "
				"lastModified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP "
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
				"name TEXT NOT NULL REFERENCES wares(name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
				"tag TEXT NOT NULL REFERENCES tags(name) "
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

	if(!tables.has("ware_types")){
		sql.exec("CREATE TABLE ware_types ("
				  "name TEXT NOT NULL REFERENCES wares(name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "type TEXT NOT NULL, "
				  "UNIQUE (name, type) "
				  ")"
			    );
		sql.exec("CREATE INDEX ware_types_name_index "
				"ON ware_types(name)");
		sql.exec("CREATE INDEX ware_types_type_index "
				"ON ware_types(type)");
	}

	cols = sql.columns("ware_types");
	if(		!cols.has("name") ||
			!cols.has("type")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table ware_types in the openend database.");
}

WareDb::~WareDb()
{
}

void WareDb::insert(const Ware & ware)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO wares (name, unit) VALUES(?, ?)");
	sqlQuery.bindValue(0, ware.name);
	sqlQuery.bindValue(1, ware.unit);
	sqlQuery.exec();

	unsigned i, s = ware.tags.size();
	sqlQuery.prepare("INSERT INTO ware_tags (name, tag) VALUES(?, ?)");
	for(i=0; i<s; i++){
		const Text & tag = ware.tags.queryAt(i);
		sqlQuery.bindValue(0, ware.name);
		sqlQuery.bindValue(1, tag);
		sqlQuery.exec();
	}

	s = ware.types.size();
	sqlQuery.prepare("INSERT INTO ware_types (name, type) VALUES(?, ?)");
	for(i=0; i<s; i++){
		const Text & type = ware.types.queryAt(i);
		sqlQuery.bindValue(0, ware.name);
		sqlQuery.bindValue(1, type);
		sqlQuery.exec();
	}

	tr.commit();
}

void WareDb::update(const Ware & orig, const Ware & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* FIXME check if orig and modified are really different. Note: we need this check here
	 * because a ware object is saved into multiple tables and some tables might require
	 * update some might not. We also need it because it is important to avoid unneccessary
	 * network traffic. */
	sqlQuery.prepare("UPDATE wares SET name = ?, unit = ? WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.unit);
	sqlQuery.bindValue(2, orig.name);
	sqlQuery.exec();

	unsigned i, s;
	s = modified.tags.size();
	sqlQuery.prepare("INSERT INTO ware_tags (name, tag) VALUES(?, ?)");
	for(i=0; i<s; i++){
		const Text & tag = modified.tags.queryAt(i);
		if(orig.tags.has(tag))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, tag);
		sqlQuery.exec();
	}
	s = orig.tags.size();
	sqlQuery.prepare("DELETE FROM ware_tags WHERE name = ? AND tag = ?");
	for(i=0; i<s; i++){
		const Text & tag = orig.tags.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If wares table is updated already.) */
		if(!modified.tags.has(tag)){
			sqlQuery.bindValue(0, modified.name);
			sqlQuery.bindValue(1, tag);
			sqlQuery.exec();
		}
	}

	s = modified.types.size();
	sqlQuery.prepare("INSERT INTO ware_types (name, type) VALUES(?, ?)");
	for(i=0; i<s; i++){
		const Text & type = modified.types.queryAt(i);
		if(orig.types.has(type))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, type);
		sqlQuery.exec();
	}
	s = orig.types.size();
	sqlQuery.prepare("DELETE FROM ware_types WHERE name = ? AND type = ?");
	for(i=0; i<s; i++){
		const Text & type = orig.types.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If Wares table is updated already.) */
		if(!modified.types.has(type)){
			sqlQuery.bindValue(0, modified.name);
			sqlQuery.bindValue(1, type);
			sqlQuery.exec();
		}
	}

	tr.commit();
}

void WareDb::del(const Ware & ware)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM wares WHERE name = ?");
	sqlQuery.bindValue(0, ware.name);
	sqlQuery.exec();

	tr.commit();
}

void WareDb::query(WareSet & wares)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT name, unit FROM wares");
	sqlQuery.exec();
	wares.clear();
	int nameNo = sqlQuery.colIndex("name");
	int unitNo = sqlQuery.colIndex("unit");
	DBG("----- WareName query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Ware *ware = new Ware();
		ware->name = sqlQuery.text(nameNo);
		ware->unit = sqlQuery.text(unitNo);
		wares.add(ware);
	}
	DBG("-----");

	sqlQuery.exec("SELECT name, tag FROM ware_tags");
	nameNo = sqlQuery.colIndex("name");
	int tagNo = sqlQuery.colIndex("tag");
	DBG("----- Ware tags query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Text name(sqlQuery.text(nameNo));
		if(wares.has(name))
			wares.query(name).tags.add(new Text(sqlQuery.text(tagNo)));
	}
	DBG("-----");

	sqlQuery.exec("SELECT name, type FROM ware_types");

	nameNo = sqlQuery.colIndex("name");
	int typeNo = sqlQuery.colIndex("type");

	DBG("----- Ware types query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Text name(sqlQuery.text(nameNo));
		if(wares.has(name))
			wares.query(name).types.add(
					new Text(sqlQuery.text(typeNo)));
	}
	DBG("-----");

	tr.commit();
}
