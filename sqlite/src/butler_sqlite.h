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

	/* TODO :
	 * - use datetime for uploaded field (not just date)
	 * - check date to string conversion to be consistent with db and
	 *   independent of locale
	 * - escape strings going into tables
	 * - ensure transactionality
	 * - begin and close and rollback transactions automatically
	 * - update of tag(name) should result in update of every occurances of
	 *   that tag in the database. Is there some help for this in SQL?
	 * - check if object has changed in database before
	 *   insert/update/delete operations
	 */

	class Sqlite : public Db
	{
		public:
			Sqlite(const QString& path);
			virtual ~Sqlite();
		private:
			Sqlite();

			/*
			 *	Db
			 */

		public:
			bool connect();
			bool open();
			bool close();
		private:
			bool reportSqlError();
			bool initializeTables();

			/*
			 *	TagDb
			 */

		public:
			bool insertTag(const Tag &t);
			bool updateTag(const Tag &orig, const Tag &modified);
			bool deleteTag(const Tag &t);
			TagSet* queryTags();
		private:
			bool createTagsTable();
			bool checkTagsTable();

			/*
			 *	QueryOptionsDb
			 */

		public:
			bool insertQueryOptions(const QueryOptions &qo);
			bool updateQueryOptions(
					const QueryOptions &orig,
					const QueryOptions &modified);
			bool deleteQueryOptions(const QueryOptions &qo);
			TagSet* queryTags(const QueryOptions &qo);
			QueryOptions* queryQueryOptions(const QString &name);
		private:
			bool createQueriesTable();
			bool checkQueriesTable();
			bool createQueryTagsTable();
			bool checkQueryTagsTable();
			bool insertQueryTag(
					const QueryOptions &qo, const Tag &t);
			bool deleteQueryTag(
					const QueryOptions &qo, const Tag &t);
			bool insertQueryTags(const QueryOptions &qo);
			bool updateQueryTags(
					const QueryOptions &orig,
					const QueryOptions &modified);
			bool deleteQueryTags(const QueryOptions &qo);

			/*
			 *	ItemDb
			 */

		public:
			TagSet* queryTags(const Item &item);
			ItemSet* queryItems(const QueryOptions &qo);
		private:
			bool createItemsTable();
			bool checkItemsTable();
			bool insertItem(const Item &i);
			bool deleteItem(const Item &i);
			bool updateItem(
					const Item &orig,
					const Item &modified);
			bool createItemTagsTable();
			bool checkItemTagsTable();
			bool insertItemTag(const Item &i, const Tag &t);
			bool deleteItemTag(const Item &i, const Tag &t);
			bool insertItemTags(const Item &i);
			bool updateItemTags(
					const Item &orig,
					const Item &modified);
			bool deleteItemTags(const Item &i);
			bool createPurchasedItemsTable();
			bool checkPurchasedItemsTable();
			bool insertItemPurchased(const Item &i);
			bool updateItemPurchased(
					const Item &orig,
					const Item &modified);
			bool deleteItemPurchased(const Item &i);

			/*
			 *	Members
			 */

		private:
			QSqlDatabase db;
			QString path;
	};

}

#endif

