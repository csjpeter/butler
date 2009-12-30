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
	QueryOptions* queryQueryOptions(const QString &name);
	{

		/*	QList<Tag*>* Sqlite::queryQueryOptionTags()
			{
			*/
		/* rewrite this to use Queries and QueryTags table instead */
#if 0
		/* assemble query */
		QString query("SELECT name, active FROM Tags");

		/* execute query */
		QSqlQuery sqlQuery(query, *db);
		sqlQuery.exec();

		/* evaluate query result */
		int nameNo = sqlQuery.record().indexOf("name");
		int activeNo = sqlQuery.record().indexOf("active");

		QList<Tag*> *tags = new QList<Tag*>;

		qDebug("----- Query option tags from db:");

		while (sqlQuery.next()) {
			Tag *tag = new Tag();

			tag->name = QVariant(sqlQuery.value(nameNo).toString());
			if(sqlQuery.value(activeNo).toString() == "t")
				tag->checked = true;
			else
				tag->checked = false;

			tags->append(tag);

			qDebug("Tag: %s", qPrintable(tag->name.toString()));
		}

		qDebug("-----");

		return tags;
#endif
		/*
		   return NULL;
		   }
		   */
	}
}

