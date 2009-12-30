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

#include <butler_db.h>

class QSqlDatabase;

namespace Butler {

	class Sqlite : public Db
	{
		private:
			Q_OBJECT;

		private:
			Sqlite();
		public:
			Sqlite(const QString& path);
			virtual ~Sqlite();

			/*
			 *	TagDb
			 */

			QList<Tag*>* queryTags();
			QList<Tag*>* queryTags(const Item &item);
			QList<Tag*>* queryTags(const QueryOptions &qo);

			/*
			 *	ItemDb
			 */

			QList<Item*>* queryItemList(const QueryOptions &qo);

			/*
			 *	QueryOptionsDb
			 */

			QueryOptions* queryQueryOptions(const QString &name);

			/*
			 *	Db
			 */

		private:

			void reportSqlError();
			void initializeTables();

			void createItemsTable();
			void checkItemsTable();
			void createPurchasedItemsTable();
			void checkPurchasedItemsTable();
			void createTagsTable();
			void checkTagsTable();
			void createItemTagsTable();
			void checkItemTagsTable();
			void createQueriesTable();
			void checkQueriesTable();
			void createQueryTagsTable();
			void checkQueryTagsTable();

		private:
			QSqlDatabase *db;
			QString path; /* path of db file */
	};

}

#endif

