/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_QUERYTAGS_DB_H
#define BUTLER_SQLITE_QUERYTAGS_DB_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_tag.h"

namespace Butler {
namespace Sqlite {

	class QueryTagsDb
	{
		public:
			QueryTagsDb(Db &_db, TagDb &_tagDb);
			virtual ~QueryTagsDb();
		private:
			QueryTagsDb();

		public:
			bool initializeTables(QStringList &tables);
			bool createQueryTagsTable();
			bool checkQueryTagsTable();
			bool insertQueryTag(const Query &qo, const Tag &t);
			bool deleteQueryTag(const Query &qo, const Tag &t);
			bool insertQueryTags(const Query &qo);
			bool updateQueryTags(
					const Query &orig,
					const Query &modified);
			bool deleteQueryTags(const Query &qo);
			TagSet* queryQueryTags(const Query &qo);

		private:
			Db &db;
			TagDb &tagDb;
	};
}
}

#endif


