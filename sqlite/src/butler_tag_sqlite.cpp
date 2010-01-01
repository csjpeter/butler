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

#include <ButlerDebug>
#include <ButlerSqlite>

namespace Butler {
	TagSet* Sqlite::queryTags()
	{
		ENTER_FUNCTION();
		/* assemble query */
		QString query("SELECT name FROM Tags");

		/* execute query */
		QSqlQuery sqlQuery(query, db);
		sqlQuery.exec();

		/* evaluate query result */
		int nameNo = sqlQuery.record().indexOf("name");

		TagSet *tags = new TagSet();

		v1Debug("----- Reading all tags from db:");
		while (sqlQuery.next()) {
			Tag *tag = new Tag(sqlQuery.value(nameNo).toString());
			tags->append(tag);
			v1Debug("Tag: %s", qPrintable(tag->name));
		}
		v1Debug("-----");

		LEAVE_FUNCTION();
		return tags;
	}

	TagSet* Sqlite::queryTags(const Item& item)
	{
		ENTER_FUNCTION();

		/*FIXME: begin transaction*/

		TagSet* tags = queryTags();

		/* assemble query */
		QString query("SELECT tag FROM ItemTags WHERE name=");
		query.append(item.name);

		/* execute query */
		QSqlQuery sqlQuery(query, db);
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

	TagSet* Sqlite::queryTags(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		Q_UNUSED(qo);
		return new TagSet;
		LEAVE_FUNCTION();
	}
}

