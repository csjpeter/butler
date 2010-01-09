/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_DB_H
#define BUTLER_SQLITE_DB_H

#include <QObject>
#include <QSqlDatabase>

#include <ButlerDb>
#include <ButlerQuery>
#include <ButlerQuerySet>
#include <ButlerItem>
#include <ButlerItemSet>

namespace Butler {
namespace Sqlite {

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

	/* Idea :
	 * - ERR values to support:
	 *   UPDATE_ERROR_OBJECT_NOT_EXISTS
	 *   UPDATE_ERROR_OBJECT_CHANGED
	 *   UNSPECIFIED_ERROR
	 *   OPEN_ERROR_OLD_DATABASE_SCHEMA
	 */

	class Db
	{
		public:
			Db(const QString& path);
			~Db();
		private:
			Db();

		public:
			bool connect();
			bool open();
			bool close();
			const QString& lastError();
			bool reportSqlError();

		public:
			QSqlDatabase db;
			QString path;
			QString lastErr;
	};
}
}

#endif

