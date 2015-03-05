/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */
#if 0
#include "butler_tag_db.h"

TagDb::TagDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("tags"))
		sql.exec(	"CREATE TABLE tags ("
				"name TEXT, "
				"description TEXT NOT NULL DEFAULT '', "
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
				"" // keys
				"PRIMARY KEY (name)"
				")"
				);
	/*sql.exec("CREATE TRIGGER update_last_modified "
		"BEFORE INSERT OR UPDATE ON tags "
		"BEGIN "
			"FOR EACH ROW "
			"IF (NEW != OLD) THEN "
				"NEW.last_modified = CURRENT_TIMESTAMP; "
				"NEW.last_modified = datetime('now'); "
			"END IF; "
		"END;");*/

	cols = sql.columns("tags");
	if(	!cols.has("name") ||
		!cols.has("description") ||
		!cols.has("last_modified")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table tags in the openend database.");
}

TagDb::~TagDb()
{
}
/*
UPDATE table SET field='C', field2='Z' WHERE id=3;
INSERT INTO table (id, field, field2)
	SELECT 3, 'C', 'Z' WHERE NOT EXISTS (SELECT 1 FROM table WHERE id=3);
*/

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
#endif

