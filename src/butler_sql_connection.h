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


class DatabaseDescriptor
{
	public:

	csjp::String name;	// will be the connection name
	SqlDriver driver;	// for example SQLite or PSql
	csjp::String databaseName;	// file name in case of sqlite
	csjp::String username;
	csjp::String password;
	YNBool savePassword;
	csjp::String host;	// host name or ip
	UInt port;
	enum Fields {
		Name,
		Driver,
		DatabaseName,
		Username,
		Password,
		SavePassword,
		Host,
		Port,
		NumOfFields

		, NumOfAllFields
	};

public:
	DatabaseDescriptor() {}

	explicit DatabaseDescriptor(const csjp::String & name) :
		name(name)
	{}

	explicit DatabaseDescriptor(const DatabaseDescriptor & orig) :
		name(orig.name),
		driver(orig.driver),
		databaseName(orig.databaseName),
		username(orig.username),
		password(orig.password),
		savePassword(orig.savePassword),
		host(orig.host),
		port(orig.port)
	{}

	DatabaseDescriptor(DatabaseDescriptor && temp) :
		name(csjp::move_cast(temp.name)),
		driver(csjp::move_cast(temp.driver)),
		databaseName(csjp::move_cast(temp.databaseName)),
		username(csjp::move_cast(temp.username)),
		password(csjp::move_cast(temp.password)),
		savePassword(csjp::move_cast(temp.savePassword)),
		host(csjp::move_cast(temp.host)),
		port(csjp::move_cast(temp.port))
	{}

	~DatabaseDescriptor() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= driver;
		json["driver"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= databaseName;
		json["databaseName"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= username;
		json["username"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= password;
		json["password"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= savePassword;
		json["savePassword"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= host;
		json["host"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= port;
		json["port"] = move_cast(s);
	}
		return json;
	}

	DatabaseDescriptor& operator=(DatabaseDescriptor && temp)
	{
		name = csjp::move_cast(temp.name);
		driver = csjp::move_cast(temp.driver);
		databaseName = csjp::move_cast(temp.databaseName);
		username = csjp::move_cast(temp.username);
		password = csjp::move_cast(temp.password);
		savePassword = csjp::move_cast(temp.savePassword);
		host = csjp::move_cast(temp.host);
		port = csjp::move_cast(temp.port);
		return *this;
	}

	DatabaseDescriptor& operator=(const DatabaseDescriptor & orig)
	{
		DatabaseDescriptor copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const DatabaseDescriptor & other) const
	{
		if(
				name != other.name ||
				driver != other.driver ||
				databaseName != other.databaseName ||
				username != other.username ||
				password != other.password ||
				savePassword != other.savePassword ||
				host != other.host ||
				port != other.port
			)
			return false;
		return true;
	}

	bool isLess(const DatabaseDescriptor & other) const
	{
		return name < other.name;	}

	bool isMore(const DatabaseDescriptor & other) const
	{
		return other.name < name;	}

	bool isLess(const csjp::String & othername) const
	{
		return name < othername;	}

	bool isMore(const csjp::String & othername) const
	{
		return othername < name;	}


};
inline bool operator==(const DatabaseDescriptor & a, const DatabaseDescriptor & b) { return a.isEqual(b); }
inline bool operator!=(const DatabaseDescriptor & a, const DatabaseDescriptor & b) { return !a.isEqual(b); }
inline bool operator<(const DatabaseDescriptor & a, const DatabaseDescriptor & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const DatabaseDescriptor & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const csjp::String & a, const DatabaseDescriptor & b) { return b.isMore(a); }
inline bool operator<(const DatabaseDescriptor & a, const csjp::String & b) { return a.isLess(b); }

class DatabaseDescriptorSet : public csjp::SorterOwnerContainer<DatabaseDescriptor>
{
	public:
	csjp::PodArray<DatabaseDescriptor::Fields> ordering;
	bool ascending;

	const DatabaseDescriptorSet & operator=(DatabaseDescriptorSet && temp)
	{
		csjp::SorterOwnerContainer<DatabaseDescriptor>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const DatabaseDescriptor & a, const DatabaseDescriptor & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case DatabaseDescriptor::Name :
				ret = a.name < b.name;
				break;
			case DatabaseDescriptor::Driver :
				ret = a.driver < b.driver;
				break;
			case DatabaseDescriptor::DatabaseName :
				ret = a.databaseName < b.databaseName;
				break;
			case DatabaseDescriptor::Username :
				ret = a.username < b.username;
				break;
			case DatabaseDescriptor::Password :
				ret = a.password < b.password;
				break;
			case DatabaseDescriptor::SavePassword :
				ret = a.savePassword < b.savePassword;
				break;
			case DatabaseDescriptor::Host :
				ret = a.host < b.host;
				break;
			case DatabaseDescriptor::Port :
				ret = a.port < b.port;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	DatabaseDescriptorSet() :
		csjp::SorterOwnerContainer<DatabaseDescriptor>(),
		ascending(true)
	{
		ordering.add(DatabaseDescriptor::Name);
		ordering.add(DatabaseDescriptor::Driver);
		ordering.add(DatabaseDescriptor::DatabaseName);
		ordering.add(DatabaseDescriptor::Username);
		ordering.add(DatabaseDescriptor::Password);
		ordering.add(DatabaseDescriptor::SavePassword);
		ordering.add(DatabaseDescriptor::Host);
		ordering.add(DatabaseDescriptor::Port);
	}
	DatabaseDescriptorSet(const DatabaseDescriptorSet & ts) :
		csjp::SorterOwnerContainer<DatabaseDescriptor>(ts),
		ascending(true)
	{
		ordering.add(DatabaseDescriptor::Name);
		ordering.add(DatabaseDescriptor::Driver);
		ordering.add(DatabaseDescriptor::DatabaseName);
		ordering.add(DatabaseDescriptor::Username);
		ordering.add(DatabaseDescriptor::Password);
		ordering.add(DatabaseDescriptor::SavePassword);
		ordering.add(DatabaseDescriptor::Host);
		ordering.add(DatabaseDescriptor::Port);
	}
	~DatabaseDescriptorSet() {}


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

