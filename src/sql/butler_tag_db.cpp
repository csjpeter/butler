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
	SqlQuery insertQuery(sql);
	SqlTransaction tr(sql);

	insertQuery.prepare("INSERT INTO tags (name, description) VALUES (?, ?)");
	insertQuery.bindValue(0, t.name);
	insertQuery.bindValue(1, t.description);
	insertQuery.exec();

	tr.commit();
}

void TagDb::update(const Tag & orig, const Tag & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE tags SET name = ?, description = ? WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.description);
	sqlQuery.bindValue(2, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void TagDb::del(const Tag & t)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM tags WHERE name = ?");
	sqlQuery.bindValue(0, t.name);
	sqlQuery.exec();

	tr.commit();
}

void TagDb::query(TagSet & tags)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT name, description FROM tags");
	sqlQuery.exec();

	tags.clear();

	int nameNo = sqlQuery.colIndex("name");
	int descriptionNo = sqlQuery.colIndex("description");

	DBG("----- Reading all tags from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Tag *tag = new Tag();
		tag->name = sqlQuery.text(nameNo);
		tag->description = sqlQuery.text(descriptionNo);
		tags.add(tag);
	}
	DBG("-----");

	tr.commit();
}
