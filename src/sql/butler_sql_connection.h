/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQL_CONNECTION_H
#define BUTLER_SQL_CONNECTION_H

#include <QObject>
#include <QSqlDatabase>

#include <csjp_reference_container.h>

#include <csjp_object.h>

#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_item.h>
#include <butler_item_set.h>

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

	QSqlQuery *createQuery();
	void exec(const QString &query);
	QSqlRecord record(const QString &tablename) const;
	QStringList tables() const;
	bool isOpen();
	void transaction();
	void commit();
	void rollback();
	QString dbErrorString();

public:
	const DatabaseDescriptor & dbDesc;

private:
	QSqlDatabase db;
	unsigned transactions;
};

#endif
