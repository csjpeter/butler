/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

#include <csjp_debug.h>

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
			CRITICAL("Incompatible table Tags "
					"in the openend database.");
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

