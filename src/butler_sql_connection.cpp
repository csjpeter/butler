/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <csjp_object.h>
#include <csjp_owner_container.h>

#include <butler_macros.h>

#include <QStringList>

#include "butler_sql_connection.h"

#ifdef PGSQL
SqlResult::SqlResult(PGresult * result) : driver(SqlDriver::PSql), row(0)
{
	this->res.pg = result;
	int rows = PQntuples(res.pg);
	if(rows <= 0)
		row = -1;
}
#endif

SqlResult::SqlResult(sqlite3_stmt * result, int status) : driver(SqlDriver::SQLite), row(0)
{
	this->res.lite = result;
	if(status == SQLITE_DONE)
		row = -1;
}

const char * SqlResult::value(int col)
{
	switch(driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			return PQgetvalue(res.pg, row, col);
#endif
		case SqlDriver::SQLite :
			return (const char*)sqlite3_column_text(res.lite, col);
#ifdef MYSQL
		case SqlDriver::MySQL :
			return 0;
#endif
	}
	throw csjp::ShouldNeverReached(EXCLI);
}

bool SqlResult::nextRow()
{
	switch(driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			{
				int rows = PQntuples(res.pg);
				if(rows <= row)
					return false;
				row++;
				if(rows <= row)
					return false;
				return true;
			}
#endif
		case SqlDriver::SQLite :
			{
				int err = sqlite3_step(res.lite);
				if(err == SQLITE_ROW){
					row++;
					return true;
				}
				return false;
			}
#ifdef MYSQL
		case SqlDriver::MySQL :
			return false;
#endif
	}
	throw csjp::ShouldNeverReached(EXCLI);
}

SqlResult::~SqlResult()
{
	switch(driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			PQclear(res.pg);
			break;
#endif
		case SqlDriver::SQLite :
			sqlite3_finalize(res.lite);
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			res.mysql = 0;
			break;
#endif
	}
}

SqlConnection::SqlConnection(const DatabaseDescriptor & _dbDesc) :
	transactions(0),
	dbDesc(_dbDesc),
	desc(dbDesc)
{
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			conn.pg = 0;
			break;
#endif
		case SqlDriver::SQLite :
			conn.lite = 0;
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			conn.mysql = 0;
			break;
#endif
	}
	open();
}

SqlConnection::~SqlConnection()
{
	try {
		close();
	} catch (csjp::Exception & e) {
		LOG("%", e);
	} catch (std::exception & e) {
		LOG("%", e);
	}
}

bool SqlConnection::isOpen()
{
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			{
				if(!conn.pg)
					return false;
				int state = PQstatus(conn.pg);
				switch(state){
					case CONNECTION_OK:
						return true;
					case CONNECTION_BAD:
						close();
						return false;
					default:
						close();
						throw csjp::LogicError("Unexpected connection state %.", state);
				}
			}
			break;
#endif
		case SqlDriver::SQLite :
			return conn.lite != 0;
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
#endif
	}
	return false;
}

void SqlConnection::open()
{
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			{
				csjp::String connStr;
				connStr.cat("postgres://",desc.username,":",desc.password,"@",
							desc.host,":",desc.port,"/",desc.databaseName);
				conn.pg = PQconnectdb(connStr.str);
				if(PQstatus(conn.pg) != CONNECTION_OK){
					PQfinish(conn.pg);
					csjp::String str;
					str.cat("postgres://",desc.username,"@",
							desc.host,":",desc.port,"/",desc.databaseName);
					throw DbError("Failed to connect to postgresql with connection string:\n"
							"%\nError:\n%", str, PQerrorMessage(conn.pg));
				}
				int clientEncoding = PQclientEncoding(conn.pg);
				LOG("PG client encoding: % - %",
						clientEncoding, pg_encoding_to_char(clientEncoding));
				//extern int PQsetClientEncoding(PGconn *conn, const char *encoding);
				//int encoding = pg_char_to_encoding(const char *name);
				int serverEncoding = pg_valid_server_encoding_id(clientEncoding);
				LOG("ServerEncoding % - %", serverEncoding, pg_encoding_to_char(serverEncoding));
			}
			break;
#endif
		case SqlDriver::SQLite :
			{
				int res = sqlite3_open_v2(desc.databaseName.str, &(conn.lite),
						SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
				if(res != SQLITE_OK || conn.lite == 0)
					throw DbError("Failed to open sqlite database/file '%'\nError:\n%",
							desc.databaseName, sqlite3_errstr(res));
				exec("PRAGMA foreign_keys = ON");
				//exec("PRAGMA foreign_keys"); // query the value
			}
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
#endif
	}
	ENSURE(isOpen(), csjp::LogicError);
}

void SqlConnection::close()
{
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			PQfinish(conn.pg);
			conn.pg = 0;
			break;
#endif
		case SqlDriver::SQLite :
			{
				int res = sqlite3_close(conn.lite);
				if(res < 0)
					throw DbError("Failed to close sqlite database/file '%'\n", desc.databaseName);
				conn.lite = 0;
			}
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
#endif
	}
	ENSURE(!isOpen(), csjp::LogicError);
}

SqlResult SqlConnection::exec(const csjp::Array<csjp::String> & params, const char * query)
{
	if(!isOpen()) open();
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			{
				csjp::String cmd(query);
				auto s = params.size();
				for(size_t i = 1; i <= s; i++){
					csjp::String p("$");
					p << i;
					cmd.replace("?", p.str, 0, cmd.length, 1);
				}

				csjp::PodArray<const char *> paramValues;
				//LOG("Query: ", cmd);
				//int i = 1;
				for(const auto & p : params){
					paramValues.add(p.str);
					//LOG("Param %: '%'", i++, p);
				}

				PGresult * res = PQexecParams(
							conn.pg, cmd.str, paramValues.length, 0, paramValues.data, 0, 0, 0);
				/*if(!res)
				  throw DbError("Fatal, the sql cmd.str result is null. Error message:\n%",
				  PQerrorMessage(conn.pg));*/
				const char * errMsg = PQerrorMessage(conn.pg);
				int status = PQresultStatus(res);
				if(status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
					PQclear(res);
					throw DbError("Failed PgSql query:\n%\n"
							"Status:%\nError message:\n%", cmd.str, status, errMsg);
				}
				try {
					return SqlResult(res);
				} catch (std::exception & e) {
					PQclear(res);
					throw;
				}
			}
			break;
#endif
		case SqlDriver::SQLite :
			{
				sqlite3_stmt *ppStmt = 0;
				int status = sqlite3_prepare_v2(conn.lite, query, strlen(query), &ppStmt, 0);
				if(status != SQLITE_OK){
					DbError e("Failed to prepare sqlite query:\n%\nError:\n%",
							query, sqlite3_errstr(status));
					sqlite3_finalize(ppStmt);
					throw e;
				}
				//LOG("Query: %", query);
				int i = 1;
				for(const auto & p : params){
					//LOG("Param %: '%'", i, p);
					status = sqlite3_bind_text(ppStmt, i++, p.str, p.length, SQLITE_TRANSIENT);
					if(status != SQLITE_OK){
						DbError e("Failed to bind parameter '%' to sqlite query:\n%"
								"\nError: %\n%", p, query, status, sqlite3_errstr(status));
						sqlite3_finalize(ppStmt);
						throw e;
					}
				}
				status = sqlite3_step(ppStmt);
				if(status != SQLITE_OK && status != SQLITE_DONE && status != SQLITE_ROW){
					DbError e("Failed to execute sqlite query:\n%\nError: %\n%",
							query, status, sqlite3_errstr(status));
					sqlite3_finalize(ppStmt);
					throw e;
				}
				try {
					return SqlResult(ppStmt, status);
				} catch (std::exception & e) {
					sqlite3_finalize(ppStmt);
					throw;
				}
			}
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
#endif
	}
	throw csjp::ShouldNeverReached(EXCLI);
}

SqlTableNames SqlConnection::tables()
{
	SqlTableNames tableNames;
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			{
				SqlResult result = exec("SELECT table_name FROM information_schema.tables "
						"WHERE table_schema = 'public' ORDER BY table_name;");
				for(auto & row : result)
					tableNames.add(new csjp::String(row.value(0)));
			}
			break;
#endif
		case SqlDriver::SQLite :
			{
				SqlResult result = exec("SELECT name FROM sqlite_master WHERE type='table'");
				for(auto & row : result)
					tableNames.add(new csjp::String(row.value(0)));
			}
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
#endif
	}
	return tableNames;
}

SqlColumns SqlConnection::columns(const char * tablename)
{
	SqlColumns cols;
	switch(desc.driver){
#ifdef PGSQL
		case SqlDriver::PSql :
			{
				SqlResult result = exec("SELECT column_name "
						"FROM information_schema.columns "
						"WHERE table_schema='public' AND table_name=$1;", tablename);
				for(auto & row : result)
					cols.add(new csjp::String(row.value(0)));
			}
			break;
#endif
		case SqlDriver::SQLite :
			{
				csjp::String query;
				query.cat("PRAGMA table_info('", tablename, "')");
				SqlResult result = exec(query);
				for(auto & row : result)
					cols.add(new csjp::String(row.value(1)));
			}
			break;
#ifdef MYSQL
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
#endif
	}
	return cols;
}

SqlTransaction::SqlTransaction(SqlConnection & sql) :
	sql(sql),
	committed(false)
{
	if(sql.transactions == 0)
		sql.exec("BEGIN TRANSACTION");
	sql.transactions++;
}

SqlTransaction::~SqlTransaction()
{
	if(committed)
		return;
	if(sql.transactions == 1)
		sql.exec("ROLLBACK");
	sql.transactions--;
}

void SqlTransaction::commit()
{
	if(committed)
		return;
	committed = true;
	if(sql.transactions == 1)
		sql.exec("COMMIT");
	sql.transactions--;
}

