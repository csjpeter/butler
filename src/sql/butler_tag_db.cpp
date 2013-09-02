/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_tag_db.h"

TagDb::TagDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("tags"))
		sql.exec(	"CREATE TABLE tags ("
				"name VARCHAR(64) PRIMARY KEY, "
				"description TEXT NOT NULL DEFAULT ''"
				")"
				);

	cols = sql.columns("tags");
	if(	!cols.has("name") ||
		!cols.has("description")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table tags in the openend database.");
}

TagDb::~TagDb()
{
}

void TagDb::insert(const Tag & t)
{
	SqlTransaction tr(sql);

	SqlQuery insertQuery(sql);
	insertQuery.prepare("INSERT INTO tags (name, description) VALUES (?, ?)");
	insertQuery.bindValue(0, t.name);
	insertQuery.bindValue(1, t.description);
	insertQuery.exec();

	tr.commit();
}

void TagDb::update(const Tag & orig, const Tag & modified)
{
	SqlTransaction tr(sql);

	SqlQuery updateQuery(sql);
	updateQuery.prepare("UPDATE tags SET name = ?, description = ? WHERE name = ?");
	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.description);
	updateQuery.bindValue(2, orig.name);
	updateQuery.exec();

	tr.commit();
}

void TagDb::del(const Tag & t)
{
	SqlTransaction tr(sql);

	SqlQuery deleteQuery(sql);
	deleteQuery.prepare("DELETE FROM tags WHERE name = ?");
	deleteQuery.bindValue(0, t.name);
	deleteQuery.exec();

	tr.commit();
}

void TagDb::query(TagSet & tags)
{
	SqlTransaction tr(sql);

	SqlQuery selectQuery(sql);
	selectQuery.prepare("SELECT name, description FROM tags");
	selectQuery.exec();

	tags.clear();

	int nameNo = selectQuery.colIndex("name");
	int descriptionNo = selectQuery.colIndex("description");

	DBG("----- Reading all tags from db:");
	while (selectQuery.next()) {
		DBG("Next row");
		Tag *tag = new Tag();
		tag->name <<= selectQuery.value(nameNo).toString();
		tag->description <<= selectQuery.value(descriptionNo).toString();
		tags.add(tag);
	}
	DBG("-----");

	tr.commit();
}
