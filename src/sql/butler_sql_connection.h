/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQL_CONNECTION_H
#define BUTLER_SQL_CONNECTION_H

#include <QObject>
#include <QSqlDatabase>

#include <csjp_reference_container.h>

#include <butler_db.h>
#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_item.h>
#include <butler_item_set.h>

#include <butler_database_description.h>

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

class SqlConnection
{
public:
	SqlConnection(const DatabaseDescription & dbDesc);
	~SqlConnection();
private:
	SqlConnection();

public:
	void open();
	void close();

	QSqlQuery *createQuery();
	QSqlRecord record(const QString &tablename) const;
	QStringList tables() const;
	bool isOpen();
	void transaction();
	void commit();
	void rollback();
	QString dbErrorString();

	void addSqlCloseListener(SqlCloseListener &l);
	void removeSqlCloseListener(SqlCloseListener &l);
	void notifySqlCloseListeners();
#if 0
	void notifySqlFinishListeners();
#endif

public:
#if 0
	Container<SqlFinishListener> sqlFinishListeners;
#endif
	csjp::ReferenceContainer<SqlCloseListener> sqlCloseListeners;

private:
	const DatabaseDescription & dbDesc;
	QSqlDatabase db;
	unsigned transactions;
};

#endif
