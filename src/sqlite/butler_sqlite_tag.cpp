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

#include "butler_sqlite_tag.h"

namespace Sqlite {

TagTable::TagTable(Sql &sql) :
	sql(sql),
	insertQuery(sql),
	updateQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

TagTable::~TagTable()
{
}

void TagTable::check(QStringList &tables)
{
	if(!tables.contains("Tags"))
		sql.exec(	"CREATE TABLE Tags ("
				"name VARCHAR(64) PRIMARY KEY, "
				"desc TEXT NOT NULL DEFAULT ''"
				")"
				);

	QSqlRecord table = sql.record("Tags");
	if(	!table.contains("name") ||
		!table.contains("desc")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table Tags in the openend database.");
}

void TagTable::insert(const Tag &t)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Tags (name, desc) VALUES (?, ?)");

	insertQuery.bindValue(0, t.name);
	insertQuery.bindValue(1, t.description);
	insertQuery.exec();
	insertQuery.finish();
}

void TagTable::update(const Tag &orig, const Tag &modified)
{
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Tags SET name = ?, desc = ?"
				" WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.description);
	updateQuery.bindValue(2, orig.name);
	updateQuery.exec();
	updateQuery.finish();
}

void TagTable::del(const Tag &t)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare("DELETE FROM Tags WHERE name = ?");

	deleteQuery.bindValue(0, t.name);
	deleteQuery.exec();
	deleteQuery.finish();
}

void TagTable::query(TagSet &tags)
{
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT name, desc FROM Tags");

	selectQuery.exec();

	tags.clear();
	
	int nameNo = selectQuery.colIndex("name");
	int descNo = selectQuery.colIndex("desc");

	DBG("----- Reading all tags from db:");
	while (selectQuery.next()) {
		DBG("Next row");
		Tag *tag = new Tag(selectQuery.value(nameNo).toString());
		tag->description = selectQuery.value(descNo).toString();
		tags.add(tag);
//		tags.treeValidity();
	}
	DBG("-----");

	selectQuery.finish();
}

}

