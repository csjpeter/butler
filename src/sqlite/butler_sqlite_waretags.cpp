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

#include "butler_sqlite_waretags.h"

namespace Sqlite {

WareTagsTable::WareTagsTable(Sql &_sql) :
	sql(_sql),
	insertQuery(sql),
	deleteQuery(sql),
	selectQuery(sql)
{
}

WareTagsTable::~WareTagsTable()
{
}

bool WareTagsTable::create()
{
	bool ret = true;

	ret = ret && sql.exec("CREATE TABLE WareTags ("
			"name VARCHAR(64) NOT NULL REFERENCES Wares(name) "
				"ON DELETE CASCADE ON UPDATE CASCADE, "
			"tag VARCHAR(64) NOT NULL REFERENCES Tags(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
			"UNIQUE (name, tag) "
			")"
			);
	ret = ret && sql.exec("CREATE INDEX WareTagsNameIndex "
			"ON WareTags(name)");
	ret = ret && sql.exec("CREATE INDEX WareTagsTagIndex "
			"ON WareTags(tag)");

	return ret;
}

bool WareTagsTable::check(QStringList &tables)
{
	bool ret = true;

	ret = tables.contains("WareTags");

	if(ret){
		QSqlRecord table = sql.record("WareTags");
		if(		!table.contains("name") ||
				!table.contains("tag")
		  ) {
			ret = false;
			CRITICAL("Incompatible table WareTags "
					"in the openend database.");
		}
	}

	return ret;
}

bool WareTagsTable::insert(const Ware &ware, const QString &tag)
{
	bool ret = true;

	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO WareTags "
				"(name, tag) "
				"VALUES(?, ?)");

	insertQuery.bindValue(0, ware.name);
	insertQuery.bindValue(1, tag);
	ret = ret && insertQuery.exec();
	insertQuery.finish();

	return ret;
}

bool WareTagsTable::del(const Ware &ware, const QString &tag)
{
	bool ret = true;

	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM WareTags WHERE "
				"name = ? AND "
				"tag = ?");

	deleteQuery.bindValue(0, ware.name);
	deleteQuery.bindValue(1, tag);
	ret = ret && deleteQuery.exec();
	deleteQuery.finish();

	return ret;
}

bool WareTagsTable::insert(const Ware &ware)
{
	bool ret = true;

	unsigned i, s = ware.tags.size();
	for(i=0; i<s; i++){
		const QString &t = ware.tags.queryAt(i);
		ret = insert(ware, t);
		if(!ret)
			break;
	}

	return ret;
}

bool WareTagsTable::update(const Ware &orig, const Ware &modified)
{
	bool ret = true;

	unsigned i, s;
	
	s = modified.tags.size();
	for(i=0; i<s && ret; i++){
		const QString &tag = modified.tags.queryAt(i);
		if(!orig.tags.has(tag))
			ret = insert(modified, tag);
	}
	
	s = orig.tags.size();
	for(i=0; i<s && ret; i++){
		const QString &tag = orig.tags.queryAt(i);
		/* We use modified as reference to ware since the ware's name might
		 * has changed by the time this update is running.
		 * (If Wares table is updated already.) */
		if(!modified.tags.has(tag))
			ret = del(modified, tag);
	}

	return ret;
}

bool WareTagsTable::query(const Ware& ware, csjp::OwnerContainer<QString> &tags)
{
	bool ret = true;

	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT tag FROM WareTags "
				"WHERE name = ?");

	selectQuery.bindValue(0, ware.name);
	ret = ret && selectQuery.exec();

	if(ret){
		tags.clear();

		unsigned tagNo = selectQuery.colIndex("tag");

		DBG("----- Ware tags query result:");
		while (selectQuery.next()) {
			DBG("Next row");
			tags.add(new QString(selectQuery.value(tagNo).toString()));
		}
		DBG("-----");
	}

	selectQuery.finish();

	return ret;
}

}

