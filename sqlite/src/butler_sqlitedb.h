/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITEDB_H
#define BUTLER_SQLITEDB_H

#include <ButlerDb>

namespace Butler {

	class Private;

	class SqliteDb : public Db
	{
		public:
			SqliteDb(const QString& path);
			virtual ~SqliteDb();

		public:
			bool connect();
			bool open();
			bool close();
			bool create();
			bool check();
			bool update();
			enum Butler::UserDbError lastUserErrorId();
			const QString& lastUserError();
			const QString& lastError();

			/*
			 *	TagDb
			 */

		public:
			bool insertTag(const Tag &t);
			bool updateTag(const Tag &orig, const Tag &modified);
			bool deleteTag(const Tag &t);
			TagSet* queryTags();

			/*
			 *	QueryOptionsDb
			 */

		public:
			bool insertQuery(const Query &q);
			bool updateQuery(
					const Query &orig,
					const Query &modified);
			bool deleteQuery(const Query &q);
			TagSet* queryQueryTags(const Query &q);
			Query* queryQuery(const QString &name);

			/*
			 *	ItemDb
			 */

		public:
			bool insertItem(const Item &i);
			bool updateItem(
					const Item &orig,
					const Item &modified);
			bool deleteItem(const Item &i);
			TagSet* queryItemTags(const Item &item);
			ItemSet* queryItems(const Query &q);

			/*
			 *	Members
			 */

		private:
			Private *priv;
	};

}

#endif

