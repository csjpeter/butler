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
	class iterator
	{
	public:
		iterator(SqlResult & res, bool valid = true): res(res), valid(valid) {}
		iterator operator++() { iterator i(res, valid); valid = res.nextRow(); return i; }
		bool operator!=(const iterator & other) { return valid != other.valid; }
		SqlResult & operator*() const { return res; }
		//SqlResult & operator*() { return res; }
	private:
		SqlResult & res;
		bool valid;
	};
	iterator begin() { return iterator(*this); }
	iterator end() { return iterator(*this, false); }
public:
	~SqlResult();
	SqlResult(PGresult * res) : driver(SqlDriver::PSql), row(0) { this->res.pg = res; }
	SqlResult(sqlite3_stmt * res) : driver(SqlDriver::SQLite), row(0) { this->res.lite = res; }

	//int numOfRows() const { return PQntuples(res.pg); }
	//int numOfCols() const;
	char * value(int col);
	bool nextRow();
private:
	SqlResult() = delete;
private:
	SqlDriver driver;
	union {
		PGresult * pg;
		sqlite3_stmt *lite;
		void * mysql;
	} res;
	int row;
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

	SqlResult exec(const char * query);
	SqlResult exec(const csjp::String & query) { return exec(query.str); }
	SqlColumns columns(const char * tablename);
	SqlColumns columns(const csjp::String & tablename) { return columns(tablename.str); }
	const SqlTableNames & tables();

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

	template<typename Arg> void bind(csjp::Array<csjp::String> & params, const Arg & arg)
	{
		csjp::String str;
		str << arg;
		if(params.capacity == params.length)
			params.setCapacity(params.length + 1);
		params.add(move_cast(str));
	}
	template<typename Arg, typename... Args>
		void bind(csjp::Array<csjp::String> & params, const Arg & arg, const Args & ... args)
	{
		csjp::String str;
		str << arg;
		if(params.capacity == params.length)
			params.setCapacity(params.length * 2);
		params.add(move_cast(str));
		bind(params, args...);
	}
	template<typename... Args> SqlResult exec(const char * query, const Args & ... args)
	{
		csjp::Array<csjp::String> paramArray(16);
		bind(paramArray, args...);
		if(!isOpen()) open();
		switch(desc.driver){
			case SqlDriver::PSql :
				{
					csjp::PodArray<const char *> paramValues;
					//LOG("Query: %s", query);
					for(const auto & p : paramArray){
						paramValues.add(p.str);
						//LOG("Param: %s", p.str);
					}
					PGresult * res = PQexecParams(
								conn.pg, query, paramValues.length, 0, paramValues.data, 0, 0, 0);
					/*if(!res)
					  throw DbError("Fatal, the sql query result is null. Error message:\n%s",
					  PQerrorMessage(conn.pg));*/
					const char * errMsg = PQerrorMessage(conn.pg);
					int status = PQresultStatus(res);
					if(status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
						PQclear(res);
						throw DbError("Failed SQL command:\n%s\n\n"
								"Status:%d\nError message:\n%s", query, status, errMsg);
					}
					try {
						return SqlResult(res);
					} catch (std::exception & e) {
						PQclear(res);
						throw;
					}
				}
				break;
			case SqlDriver::SQLite :
				{
					char * errmsg = 0;
					//int res = sqlite3_exec(conn.lite, query, 0, 0, &errmsg);
					sqlite3_stmt *ppStmt = 0;
					int res = sqlite3_prepare_v2(conn.lite, query, strlen(query), &ppStmt, 0);
					if(res != SQLITE_OK || errmsg != 0){
						DbError e("Failed to prepare sqlite query:\n%s\n\nError:\n%s",
								query, errmsg);
						sqlite3_free(errmsg);
						sqlite3_finalize(ppStmt);
						throw e;
					}
					LOG("Query: %s", query);
					int i = 0;
					for(const auto & p : paramArray){
						LOG("Param: %s", p.str);
						res = sqlite3_bind_text(ppStmt, i++, p.str, p.length, SQLITE_TRANSIENT);
						if(res != SQLITE_OK || errmsg != 0){
							DbError e("Failed to bind parameter '%s' to sqlite query:\n%s"
									"\n\nError:\n%s", p.str, query, errmsg);
							sqlite3_free(errmsg);
							sqlite3_finalize(ppStmt);
							throw e;
						}
					}
					res = sqlite3_step(ppStmt);
					if(res != SQLITE_OK || errmsg != 0){
						DbError e("Failed to execute sqlite query:\n%s\n\nError:\n%s",
								query, errmsg);
						sqlite3_free(errmsg);
						sqlite3_finalize(ppStmt);
						throw e;
					}
					try {
						return SqlResult(ppStmt);
					} catch (std::exception & e) {
						sqlite3_finalize(ppStmt);
						throw;
					}
				}
				break;
			case SqlDriver::MySQL :
				throw csjp::NotImplemented();
				break;
		}
		Throw(csjp::ShouldNeverReached);
	}

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
