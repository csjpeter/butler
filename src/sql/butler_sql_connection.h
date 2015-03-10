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
#include <butler_dataclasses.h>

#include <QVariant>

@declare@ DatabaseDescriptor

class DatabaseDescriptor
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h



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
	void bindValue(int pos, const enum Query::StockOptions e);
	void bindValue(int pos, const enum Query::TagOptions e);
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

inline enum Query::StockOptions & operator<<= (enum Query::StockOptions & e, const SqlVariant & v)
{
	e = (enum Query::StockOptions)v.var.toInt(); return e;
}

inline enum Query::TagOptions & operator<<= (enum Query::TagOptions & e, const SqlVariant & v)
{
	e = (enum Query::TagOptions)v.var.toInt(); return e;
}

#endif
