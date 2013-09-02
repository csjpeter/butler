/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQL_CONNECTION_H
#define BUTLER_SQL_CONNECTION_H

#include <csjp_string.h>
#include <csjp_owner_container.h>

#include <csjp_text.h>

#include <QVariant>

/*
#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_item.h>
#include <butler_item_set.h>
*/
#include <butler_database_descriptor.h>

DECL_EXCEPTION(csjp::Exception, DbError);
DECL_EXCEPTION(DbError, DbIncompatibleTableError);
DECL_EXCEPTION(DbError, DbLogicError);
DECL_EXCEPTION(DbError, DbConnectError);

/* FIXME :
 * - check if object has changed in database before
 *   insert/update/delete operations
 * - escaping required while assempling item queries
 */

class SqlConnectionPrivate;

typedef csjp::OwnerContainer<csjp::String> SqlColumns;
typedef csjp::OwnerContainer<csjp::String> SqlTableNames;

class SqlConnection
{
public:
	SqlConnection(const DatabaseDescriptor & dbDesc);
	~SqlConnection();
private:
	SqlConnection();

public:
	void open();
	void close();

	void exec(const QString & query);
	SqlColumns columns(const QString & tablename) const;
	const SqlTableNames & tables() const;
	bool isOpen();
	QString dbErrorString();

public:
	DatabaseDescriptor dbDesc;

private:
	friend class SqlQuery;
	friend class SqlTransaction;
	SqlConnectionPrivate * priv;
};

class SqlTransaction
{
public:
	SqlTransaction(SqlConnection & sql);
	~SqlTransaction();
	void commit();
private:
	SqlConnection & sql;
	bool committed;
};

class SqlQueryPrivate;

class SqlQuery
{
public:
	SqlQuery(SqlConnection &);
	~SqlQuery();
private:
	SqlQuery();

public:

	void exec(const QString & query);
	void prepare(const QString & query);
	bool isPrepared();
	void bindValue(int pos, const QVariant & v);
	void exec();
	bool next();
	unsigned colIndex(const QString & name);
	QVariant value(int index);
	csjp::Text text(int index);
	void finish();

private:
	QString queryString();

private:
	SqlConnection & sql;
	SqlQueryPrivate * priv;
};

#endif
