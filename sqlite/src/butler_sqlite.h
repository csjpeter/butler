/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_H
#define BUTLER_SQLITE_H

#include <QObject>
#include <QSqlDatabase>

#include <ButlerDb>

class QSqlDatabase;

namespace Butler {

	class Sqlite : public Db
	{
		public:
			Sqlite(const QString& path);
			virtual ~Sqlite();

			/*
			 *	Db
			 */

			bool connect();
			bool open();
			bool close();

			/*
			 *	TagDb
			 */

			TagSet* queryTags();
			TagSet* queryTags(const Item &item);
			TagSet* queryTags(const QueryOptions &qo);

			/*
			 *	QueryOptionsDb
			 */

			QueryOptions* queryQueryOptions(const QString &name);

			/*
			 *	ItemDb
			 */

			ItemSet* queryItems(const QueryOptions &qo);

		private:
//			Sqlite();

			bool reportSqlError();
			bool initializeTables();

			bool createTagsTable();
			bool checkTagsTable();

			bool createQueriesTable();
			bool checkQueriesTable();
			bool createQueryTagsTable();
			bool checkQueryTagsTable();

			bool createItemsTable();
			bool checkItemsTable();
			bool createItemTagsTable();
			bool checkItemTagsTable();
			bool createPurchasedItemsTable();
			bool checkPurchasedItemsTable();

		private:
			QSqlDatabase db;
			QString path;
	};

}

#endif

