/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_SQL_CONNECTION_H
#define BUTLER_SQL_CONNECTION_H

#include <postgresql/libpq-fe.h>
#include <sqlite3.h>
//#include <sqlite3ext.h>

#include <csjp_string.h>
#include <csjp_pod_array.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>
#include <csjp_exception.h>

#include <butler_datetime.h>
#include <butler_text.h>
#include <butler_types.h>

#include <QVariant>

@declare@ DatabaseDescriptor
class DatabaseDescriptor
{
	@include@ dataclass_members.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

inline bool operator<(const char * lhs, const DatabaseDescriptor & rhs)
{
	return lhs < rhs.name;
}

inline bool operator<(const DatabaseDescriptor & lhs, const char * rhs)
{
	return lhs.name < rhs;
}



DECL_EXCEPTION(csjp::Exception, DbError);
DECL_EXCEPTION(DbError, DbIncompatibleTableError);
DECL_EXCEPTION(DbError, DbLogicError);
DECL_EXCEPTION(DbError, DbConnectError);

/* FIXME :
 * - check if object has changed in database before
 *   insert/update/delete operations
 * - escaping required while assempling item queries
 */

class SqlResult
{
public:
	~SqlResult();
	SqlResult(PGresult * res) : driver(SqlDriver::PSql) { this->res.pg = res; }
	SqlResult(sqlite3_stmt * res) : driver(SqlDriver::SQLite) { this->res.lite = res; }

	int numOfRows() { return PQntuples(res.pg); }
	int numOfCols() { return PQnfields(res.pg); }
	char * getValue(int row, int col) { return PQgetvalue(res.pg, row, col); }
private:
	SqlResult() = delete;
private:
	SqlDriver driver;
	union {
		PGresult * pg;
		sqlite3_stmt *lite;
		void * mysql;
	} res;
};

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
	bool isOpen();
	void open();
	void close();

	SqlResult exec(const char * query, csjp::unint len);
	SqlResult exec(const char * q) { ENSURE(q, csjp::InvalidArgument); return exec(q, strlen(q)); }
	SqlResult exec(const csjp::String & query) { return exec(query.str, query.length); }
	SqlColumns columns(const QString & tablename) const;
	const SqlTableNames & tables() const;
	//QString dbErrorString();

private:
	friend class SqlTransaction;
	unsigned transactions;
	DatabaseDescriptor dbDesc;
	//friend class SqlQuery;
	mutable SqlTableNames tableNames;

public:
	const DatabaseDescriptor & desc;
	union {
		PGconn * pg;
		sqlite3 * lite;
		void * mysql;
	} conn;
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

class SqlVariant
{
public:
	explicit SqlVariant(const QVariant & v) : var(v) {}

	QVariant var;
};

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
	void bindValue(int pos, const Text & text);
	void bindValue(int pos, const DateTime & time);
	void bindValue(int pos, const double d);
	void bindValue(int pos, const int i);
	void bindValue(int pos, const enum QueryStockOptions e);
	void bindValue(int pos, const enum QueryTagOptions e);
	void exec();
	bool next();
	unsigned colIndex(const QString & name);
	DateTime dateTime(int index);
	Text text(int index);
	double real(int index);
	int number(int index);
	SqlVariant sqlValue(int index);
	QVariant value(int index);
	void finish();

private:
	QString queryString();
	void bindValue(int pos, const QVariant & v);

private:
	SqlConnection & sql;
	bool prepared;
};

inline Text & operator<<= (Text & qstr, const SqlVariant & v)
{
	qstr = v.var.toString(); return qstr;
}

inline DateTime & operator<<= (DateTime & time, const SqlVariant & v)
{
	time = v.var.toDateTime();
	time.setTimeSpec(Qt::UTC);
	return time;
}

inline int & operator<<= (int & i, const SqlVariant & v)
{
	i = v.var.toInt(); return i;
}

inline double & operator<<= (double & d, const SqlVariant & v)
{
	d = v.var.toDouble(); return d;
}

inline bool & operator<<= (bool & b, const SqlVariant & v)
{
	QChar c = v.var.toChar();
	if(c == 'I')
		b = true;
	else
		b = false;
	return b;
}

inline enum QueryStockOptions & operator<<= (enum QueryStockOptions & e, const SqlVariant & v)
{
	e = (enum QueryStockOptions)v.var.toInt(); return e;
}

inline enum QueryTagOptions & operator<<= (enum QueryTagOptions & e, const SqlVariant & v)
{
	e = (enum QueryTagOptions)v.var.toInt(); return e;
}

#endif
