/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_TAG_DB_H
#define BUTLER_SQLITE_TAG_DB_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "butler_sqlite_db.h"

namespace Butler {
namespace Sqlite {

	class TagDb
	{
		public:
			TagDb(Db &_db);
			virtual ~TagDb();
		private:
			TagDb();

		public:
			bool initializeTables(QStringList &tables);
			bool createTagsTable();
			bool checkTagsTable();
			bool insertTag(const Tag &t);
			bool updateTag(const Tag &orig, const Tag &modified);
			bool deleteTag(const Tag &t);
			TagSet* queryTags();

		private:
			Db &db;
			QSqlQuery *insertQuery;
			QSqlQuery *updateQuery;
			QSqlQuery *deleteQuery;
			QSqlQuery *selectOneQuery;
			QSqlQuery *selectAllQuery;
	};
}
}

#endif

