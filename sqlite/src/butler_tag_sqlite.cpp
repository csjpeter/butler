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

#include "butler_sqlite.h"

namespace Butler {
	QList<Tag*>* Sqlite::queryTags()
	{
		/* assemble query */
		QString query("SELECT name FROM Tags");

		/* execute query */
		QSqlQuery sqlQuery(query, *db);
		sqlQuery.exec();

		/* evaluate query result */
		int nameNo = sqlQuery.record().indexOf("name");

		QList<Tag*> *tags = new QList<Tag*>;

		qDebug("----- Reading all tags from db:");
		while (sqlQuery.next()) {
			Tag *tag = new Tag(sqlQuery.value(nameNo).toString());
			tags->append(tag);
			qDebug("Tag: %s", qPrintable(tag->name()));
		}
		qDebug("-----");
	}

	QList<Tag*>* Sqlite::queryTags(const Item& item)
	{
		(void)item;

		/* assemble query */
		QString query("SELECT tag FROM ItemTags WHERE name=");
		query.append(item.name);

		/* execute query */
		QSqlQuery sqlQuery(query, *db);
		sqlQuery.exec();

		/* evaluate query result */
		int tagNo = sqlQuery.record().indexOf("tag");

		QList<Tag*>* tags = queryTags();

		qDebug("----- Item tags query result:");

		while (sqlQuery.next()) {
			QString name = QVariant(sqlQuery.value(tagNo).toString());

			qFind(tags.begin(), tags.end(), name)

			if(!tags->contains(name))

			Tag *tag = new Tag();
			tag->checked = true;

			tags->append(tag);

			qDebug("Tag: %s", qPrintable(tag->name.toString()));
		}

		qDebug("-----");

		/* merge tag cache with item tags */
		for(QList<Tag*>::const_iterator cacheIter = tagList.constBegin();
				cacheIter != tagList.constEnd();
				cacheIter++){
			if(!tags->contains(*cacheIter))
				tags->append(*cacheIter);
		}

		return tags;
	}

	QList<Tag*>* Sqlite::queryTags(const QueryOptions &qo)
	{
	}
}

