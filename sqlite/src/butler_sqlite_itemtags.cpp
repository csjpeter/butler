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

#include "butler_sqlite_itemtags.h"

namespace Butler {
namespace Sqlite {

	ItemTagsDb::ItemTagsDb(Db &_db, TagDb &_tagDb) :
		db(_db),
		tagDb(_tagDb)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	ItemTagsDb::~ItemTagsDb()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	bool ItemTagsDb::initializeTables(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret;

		if(!tables.contains("ItemTags"))
			ret = createItemTagsTable() && ret;
		else
			ret = checkItemTagsTable() && ret;

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::createItemTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db.db);
		query.exec("CREATE TABLE ItemTags ("
				"uploaded DATE NOT NULL REFERENCES Items(uploaded), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		ret = db.reportSqlError();
		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::checkItemTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.db.record("ItemTags");
		if(		!table.contains("item_id") ||
				!table.contains("tag")
				) {
			ret = false;
			qCritical("Incompatible table ItemTags "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::insertItemTag(const Item &i, const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "INSERT INTO ItemTags (uploaded, tag) VALUES('";
		query += i.uploaded.toString();
		query += "', '";
		query += t.name;
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::deleteItemTag(const Item &i, const Tag &t)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM ItemTags WHERE uploaded = '";
		query += i.uploaded.toString();
		query += "' AND tag = '";
		query += t.name;
		query += "'";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::insertItemTags(const Item &it)
	{
		ENTER_FUNCTION();
		bool ret;

		int i, s = it.tags.size();
		for(i=0; i<s; i++){
			const Tag &t = it.tags.query(i);
			if(!t.checked)
				continue;
			ret = insertItemTag(it, t);
			if(!ret)
				break;
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::updateItemTags(
			const Item &orig, const Item &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlQuery sqlQuery(db.db);
		QString query;

		int i, s = modified.tags.size();
		for(i=0; i<s; i++){
			const Tag &m = modified.tags.query(i);
			if(!orig.tags.hasByName(m.name) && m.checked){
				ret = insertItemTag(modified, m);
				if(!ret)
					break;
				continue;
			}

			const Tag &o = orig.tags.queryByName(m.name);
			if(o.checked == m.checked)
				continue;

			if(m.checked){
				ret = insertItemTag(modified, m);
				if(!ret)
					break;
			} else { /* o.checked is true here */
				ret = deleteItemTag(orig, o);
				if(!ret)
					break;
			}
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemTagsDb::deleteItemTags(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret;
	
		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM ItemTags WHERE query_name = '";
		query += i.name;
		query += "'";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}




	TagSet* ItemTagsDb::queryItemTags(const Item& item)
	{
		ENTER_FUNCTION();

		/*FIXME: begin transaction*/

		TagSet* tags = tagDb.queryTags();

		/* assemble query */
		QString query("SELECT tag FROM ItemTags WHERE name=");
		query.append(item.name);

		/* execute query */
		QSqlQuery sqlQuery(query, db.db);
		sqlQuery.exec();

		/*FIXME: end transaction*/

		/* evaluate query result */
		int tagNo = sqlQuery.record().indexOf("tag");

		v1Debug("----- Item tags query result:");
		while (sqlQuery.next()) {
			Tag &tag = tags->queryByName(QVariant(
					  sqlQuery.value(tagNo)).toString());
			tag.checked = true;
			v1Debug("Tag: %s", qPrintable(tag.name));
		}
		v1Debug("-----");

		LEAVE_FUNCTION();
		return tags;
	}
}
}

