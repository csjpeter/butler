/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQL_CONNECTION_H
#define BUTLER_SQL_CONNECTION_H

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
	bool isOpen();
	void open();
	void close();

	void exec(const char * query);
	void exec(const csjp::String & query) { exec(query.str); }
	SqlColumns columns(const QString & tablename) const;
	const SqlTableNames & tables() const;
	//QString dbErrorString();

private:
	DatabaseDescriptor dbDesc;
	friend class SqlQuery;
	friend class SqlTransaction;
	SqlConnectionPrivate * priv;

public:
	const DatabaseDescriptor & desc;
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
	SqlQueryPrivate * priv;
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
