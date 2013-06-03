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

#include "butler_waretags_table.h"

WareTagsTable::WareTagsTable(SqlConnection &_sql) :
	sql(_sql)
{
}

WareTagsTable::~WareTagsTable()
{
}

void WareTagsTable::check(QStringList &tables)
{
	if(!tables.contains("ware_tags")){
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

	QSqlRecord table = sql.record("ware_tags");
	if(		!table.contains("name") ||
			!table.contains("tag")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table ware_tags in the openend database.");
}

void WareTagsTable::insert(const Ware &ware, const QString &tag)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO ware_tags "
				"(name, tag) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, tag);
	insertQuery.exec();
}

void WareTagsTable::del(const Ware &ware, const QString &tag)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM ware_tags WHERE "
				"name = ? AND "
				"tag = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.bindValue(1, tag);
	deleteQuery.exec();
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
		 * (If wares table is updated already.) */
		if(!modified.tags.has(tag))
			del(modified, tag);
	}
}

void WareTagsTable::query(const Ware& ware, csjp::OwnerContainer<QString> &tags)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT tag FROM ware_tags "
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
}
