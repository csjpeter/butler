/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_SQL_CONNECTION_H
#define BUTLER_SQL_CONNECTION_H

#ifdef PGSQL
#include <postgresql/libpq-fe.h>
#endif
#include <sqlite3.h>
//#include <sqlite3ext.h>

#include <csjp_string.h>
#include <csjp_pod_array.h>
#include <csjp_array.h>
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
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
};

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

class SqlResult
{
public:
	class iterator
	{
	public:
		iterator(SqlResult & res, bool valid = true): res(res), valid(valid) {}
		iterator operator++() { iterator i(res, valid); valid = res.nextRow(); return i; }
		bool operator!=(const iterator & other) { return valid != other.valid; }
		//const SqlResult & operator*() const { return res; }
		SqlResult & operator*() { return res; }
	private:
		SqlResult & res;
		bool valid;
	};
	iterator begin() { return iterator(*this, (row == 0) ? true : false); }
	iterator end() { return iterator(*this, false); }
public:
	~SqlResult();
#ifdef PGSQL
	SqlResult(PGresult * result);
#endif
	SqlResult(sqlite3_stmt * result, int status);

	const char * value(int col);
	bool nextRow();
private:
	SqlResult() = delete;
private:
	SqlDriver driver;
	union {
		sqlite3_stmt *lite;
#ifdef PGSQL
		PGresult * pg;
#endif
#ifdef MYSQL
		void * mysql;
#endif
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
	SqlTableNames tables();
	SqlColumns columns(const char * tablename);
	SqlColumns columns(const csjp::String & tablename) { return columns(tablename.c_str()); }

private:
	friend class SqlTransaction;
	unsigned transactions;
	DatabaseDescriptor dbDesc;

public:
	const DatabaseDescriptor & desc;
	union {
		sqlite3 * lite;
#ifdef PGSQL
		PGconn * pg;
#endif
#ifdef MYSQL
		void * mysql;
#endif
	} conn;

	template<typename Arg> void bind(csjp::Array<csjp::String> & params, const Arg & arg)
	{
		csjp::String str;
		str <<= arg;
		params.add(move_cast(str));
	}
	template<typename Arg, typename... Args>
		void bind(csjp::Array<csjp::String> & params, const Arg & arg, const Args & ... args)
	{
		csjp::String str;
		str <<= arg;
		params.add(move_cast(str));
		bind(params, args...);
	}
public:
	SqlResult exec(const csjp::Array<csjp::String> & params, const char * query);
	SqlResult exec(const csjp::Array<csjp::String> & params, const csjp::String & query)
			{ return exec(params, query.c_str()); }
	SqlResult exec(const char * query)
			{ csjp::Array<csjp::String> params; return exec(params, query); }
	SqlResult exec(const csjp::String & query) { return exec(query.c_str()); }
	template<typename... Args> SqlResult exec(const char * query, const Args & ... args)
	{
		csjp::Array<csjp::String> params;
		bind(params, args...);
		return exec(params, query);
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

#endif
