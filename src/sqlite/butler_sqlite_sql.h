/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SQL_H
#define BUTLER_SQLITE_SQL_H

#include <QObject>
#include <QSqlDatabase>

#include <csjp_reference_container.h>

#include <butler_db.h>
#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_item.h>
#include <butler_item_set.h>

namespace Sqlite {

/* FIXME :
 * - check if object has changed in database before
 *   insert/update/delete operations
 * - escaping required while assempling item queries
 */

class SqlCloseListener
{
	public:
		virtual void sqlCloseNotification() = 0;
};

bool operator<(const SqlCloseListener &a, const SqlCloseListener &b);

class SqlFinishListener
{
	public:
		virtual void sqlFinishNotification() = 0;
};

bool operator<(const SqlFinishListener &a, const SqlFinishListener &b);

class Sql
{
public:
	Sql(const QString& path);
	~Sql();
private:
	Sql();

public:
	void connect();
	void open();
	void close();

	QSqlQuery *createQuery();
	void exec(const QString &query);
	QSqlRecord record(const QString &tablename) const;
	QStringList tables() const;
	bool isOpen();
#if 0
	void transaction();
	void commit();
	void rollback();
#endif
	void reportSqlError();

	void addSqlCloseListener(SqlCloseListener &l);
	void removeSqlCloseListener(SqlCloseListener &l);
	void notifySqlCloseListeners();
#if 0
	void notifySqlFinishListeners();
#endif

public:
	QString path;
	enum Db::ErrorId lastErrId;
	QString lastErr;
#if 0
	Container<SqlFinishListener> sqlFinishListeners;
#endif
	csjp::ReferenceContainer<SqlCloseListener> sqlCloseListeners;

private:
	QSqlDatabase db;
	unsigned transactions;
public:
	class Transaction
	{
	public:
		Transaction() :
			committed(false)
		{
			if(transactions == 0)
				if(!db.transaction())
					throw DbError("Failed to begin transaction: %s",
							C_STR(db.lastError().text()));
			transactions++;
		}
		void commit()
		{
			if(transactions == 1)
				if(!db.commit())
					throw DbError("Failed to commit transaction: %s",
							C_STR(db.lastError().text()));
			committed = true;
		}
		~Transaction()
		{
			if(transactions == 1 && !committed){
//				notifySqlFinishListeners();
//				FIXME notify upper layer to be able to message the user and quit.
				if(!db.rollback())
					LOG("Failed to rollback transaction: %s",
							C_STR(db.lastError().text()));
			}
			transactions--;
		}
	public:
		bool committed;
	};
};
}

#endif

