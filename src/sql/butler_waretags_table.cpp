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

#include "butler_sqlite_waretags.h"

WareTagsTable::WareTagsTable(SqlConnection &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

WareTagsTable::~WareTagsTable()
{
}

void WareTagsTable::check(QStringList &tables)
{
	if(!tables.contains("WareTags")){
		sql.exec("CREATE TABLE WareTags ("
				"name VARCHAR(64) NOT NULL REFERENCES Wares(name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
				"tag VARCHAR(64) NOT NULL REFERENCES Tags(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"UNIQUE (name, tag) "
				")"
				);
		sql.exec("CREATE INDEX WareTagsNameIndex ON WareTags(name)");
		sql.exec("CREATE INDEX WareTagsTagIndex ON WareTags(tag)");
	}

	QSqlRecord table = sql.record("WareTags");
	if(		!table.contains("name") ||
			!table.contains("tag")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table WareTags in the openend database.");
}

void WareTagsTable::insert(const Ware &ware, const QString &tag)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO WareTags "
				"(name, tag) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, tag);
	insertQuery.exec();
	insertQuery.finish();
}

void WareTagsTable::del(const Ware &ware, const QString &tag)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM WareTags WHERE "
				"name = ? AND "
				"tag = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.bindValue(1, tag);
	deleteQuery.exec();
	deleteQuery.finish();
}

void WareTagsTable::insert(const Ware &ware)
{
	unsigned i, s = ware.tags.size();
	for(i=0; i<s; i++){
		const QString &t = ware.tags.queryAt(i);
		insert(ware, t);
	}
}

void WareTagsTable::update(const Ware &orig, const Ware &modified)
{
	unsigned i, s;
	
	s = modified.tags.size();
	for(i=0; i<s; i++){
		const QString &tag = modified.tags.queryAt(i);
		if(!orig.tags.has(tag))
			insert(modified, tag);
	}
	
	s = orig.tags.size();
	for(i=0; i<s; i++){
		const QString &tag = orig.tags.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If Wares table is updated already.) */
		if(!modified.tags.has(tag))
			del(modified, tag);
	}
}

void WareTagsTable::query(const Ware& ware, csjp::OwnerContainer<QString> &tags)
{
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT tag FROM WareTags "
				"WHERE name = ?");

	selectQuery.bindValue(0, ware.name);
	selectQuery.exec();

	tags.clear();

	unsigned tagNo = selectQuery.colIndex("tag");

	DBG("----- Ware tags query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		tags.add(new QString(selectQuery.value(tagNo).toString()));
	}
	DBG("-----");

	selectQuery.finish();
}
