/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QVariant>

#include "butler_tag_table.h"

TagTable::TagTable(SqlConnection &sql) :
	sql(sql)
{
}

TagTable::~TagTable()
{
}

void TagTable::check(QStringList &tables)
{
	if(!tables.contains("tags"))
		sql.exec(	"CREATE TABLE tags ("
				"name VARCHAR(64) PRIMARY KEY, "
				"description TEXT NOT NULL DEFAULT ''"
				")"
				);

	SqlColumns cols = sql.columns("tags");
	if(	!cols.has("name") ||
		!cols.has("description")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table tags in the openend database.");
}

void TagTable::insert(const Tag &t)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO tags (name, description) VALUES (?, ?)");

	insertQuery.bindValue(0, t.name);
	insertQuery.bindValue(1, t.description);
	insertQuery.exec();
}

void TagTable::update(const Tag &orig, const Tag &modified)
{
	SqlQuery updateQuery(sql);
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE tags SET name = ?, description = ?"
				" WHERE name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.description);
	updateQuery.bindValue(2, orig.name);
	updateQuery.exec();
}

void TagTable::del(const Tag &t)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare("DELETE FROM tags WHERE name = ?");

	deleteQuery.bindValue(0, t.name);
	deleteQuery.exec();
}

void TagTable::query(TagSet &tags)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT name, description FROM tags");

	selectQuery.exec();

	tags.clear();
	
	int nameNo = selectQuery.colIndex("name");
	int descriptionNo = selectQuery.colIndex("description");

	DBG("----- Reading all tags from db:");
	while (selectQuery.next()) {
		DBG("Next row");
		Tag *tag = new Tag(selectQuery.value(nameNo).toString());
		tag->description = selectQuery.value(descriptionNo).toString();
		tags.add(tag);
	}
	DBG("-----");
}
