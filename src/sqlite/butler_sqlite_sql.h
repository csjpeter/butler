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
	bool connect();
	bool open();
	bool close();
	enum Db::UserError lastUserErrorId();
	const QString& lastUserError();
	const QString& lastError();

	QSqlQuery *createQuery();
	bool exec(const QString &query);
	QSqlRecord record(const QString &tablename) const;
	QStringList tables() const;
	bool isOpen();
	bool transaction();
	bool commit();
	bool rollback();
	bool reportSqlError();

	void addSqlCloseListener(SqlCloseListener &l);
	void removeSqlCloseListener(SqlCloseListener &l);
	void notifySqlCloseListeners();
#if 0
	void notifySqlFinishListeners();
#endif

public:
	QString path;
	enum Db::UserError lastUserErrId;
	QString lastUserErr;
	QString lastErr;
#if 0
	Container<SqlFinishListener> sqlFinishListeners;
#endif
	csjp::ReferenceContainer<SqlCloseListener> sqlCloseListeners;

private:
	QSqlDatabase db;
	int transactions;
};
}

#endif

