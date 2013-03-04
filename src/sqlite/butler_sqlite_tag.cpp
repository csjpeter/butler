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

bool TagTable::create()
{
	return sql.exec("CREATE TABLE Tags ("
			"name VARCHAR(64) PRIMARY KEY, "
			"desc TEXT NOT NULL DEFAULT ''"
			")"
			);
}

bool TagTable::check(QStringList &tables)
{
	bool ret = true;

	ret = tables.contains("Tags");

	if(ret){
		QSqlRecord table = sql.record("Tags");
		if(	!table.contains("name") ||
			!table.contains("desc")
		  ) {
			ret = false;
			LOG("Incompatible table Tags in the openend database.");
		}
	}

	return ret;
}

bool TagTable::alter()
{
	return true;
}

bool TagTable::insert(const Tag &t)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		ret = insertQuery.prepare("INSERT INTO Tags (name, desc) VALUES (?, ?)");

	insertQuery.bindValue(0, t.name);
	insertQuery.bindValue(1, t.description);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool TagTable::update(const Tag &orig, const Tag &modified)
{
	bool ret = true;

	if(!updateQuery.isPrepared())
		ret = updateQuery.prepare("UPDATE Tags SET name = ?, desc = ?"
				" WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.description);
	updateQuery.bindValue(2, orig.name);
	ret = ret && updateQuery.exec();
	updateQuery.finish();

	return ret;
}

bool TagTable::del(const Tag &t)
{
	bool ret = true;

	if(!deleteQuery.isPrepared())
		ret = deleteQuery.prepare("DELETE FROM Tags WHERE name = ?");

	deleteQuery.bindValue(0, t.name);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool TagTable::query(TagSet &tags)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		ret = selectQuery.prepare("SELECT name, desc FROM Tags");

	ret = ret && selectQuery.exec();

	if(ret){
		tags.clear();
		
		int nameNo = selectQuery.colIndex("name");
		int descNo = selectQuery.colIndex("desc");

		DBG("----- Reading all tags from db:");
		while (selectQuery.next()) {
			DBG("Next row");
			Tag *tag = new Tag(selectQuery.value(nameNo).toString());
			tag->description = selectQuery.value(descNo).toString();
			tags.add(tag);
//			tags.treeValidity();
		}
		DBG("-----");
	}

	selectQuery.finish();

	return ret;
}

}

