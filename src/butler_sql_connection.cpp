/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <csjp_object.h>
#include <csjp_owner_container.h>

#include <butler_macros.h>

#include <QStringList>

#include "butler_sql_connection.h"

const char * SqlResult::value(int col)
{
	switch(driver){
		case SqlDriver::PSql :
			return PQgetvalue(res.pg, row, col);
		case SqlDriver::SQLite :
			return (const char*)sqlite3_column_text(res.lite, col);
		case SqlDriver::MySQL :
			return 0;
	}
	throw csjp::ShouldNeverReached(EXCLI);
}

bool SqlResult::nextRow()
{
	switch(driver){
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
		case SqlDriver::SQLite :
			{
				int err = sqlite3_step(res.lite);
				if(err == SQLITE_ROW){
					row++;
					return true;
				}
				return false;
			}
		case SqlDriver::MySQL :
			return false;
	}
	throw csjp::ShouldNeverReached(EXCLI);
}

SqlResult::~SqlResult()
{
	switch(driver){
		case SqlDriver::PSql :
			PQclear(res.pg);
			break;
		case SqlDriver::SQLite :
			sqlite3_finalize(res.lite);
			break;
		case SqlDriver::MySQL :
			res.mysql = 0;
			break;
	}
}

SqlConnection::SqlConnection(const DatabaseDescriptor & _dbDesc) :
	transactions(0),
	dbDesc(_dbDesc),
	desc(dbDesc)
{
	switch(desc.driver){
		case SqlDriver::PSql :
			conn.pg = 0;
			break;
		case SqlDriver::SQLite :
			conn.lite = 0;
			break;
		case SqlDriver::MySQL :
			conn.mysql = 0;
			break;
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
		case SqlDriver::SQLite :
			return conn.lite != 0;
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
	}
	return false;
}

void SqlConnection::open()
{
	switch(desc.driver){
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
			}
			break;
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
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
	}
	ENSURE(isOpen(), csjp::LogicError);
}

void SqlConnection::close()
{
	switch(desc.driver){
		case SqlDriver::PSql :
			PQfinish(conn.pg);
			conn.pg = 0;
			break;
		case SqlDriver::SQLite :
			{
				int res = sqlite3_close(conn.lite);
				if(res < 0)
					throw DbError("Failed to close sqlite database/file '%'\n", desc.databaseName);
				conn.lite = 0;
			}
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
	}
	ENSURE(!isOpen(), csjp::LogicError);
}

SqlResult SqlConnection::exec(const csjp::Array<csjp::String> & params, const char * query)
{
	if(!isOpen()) open();
	switch(desc.driver){
		case SqlDriver::PSql :
			{
				csjp::PodArray<const char *> paramValues;
				//LOG("Query: %", query);
				for(const auto & p : params){
					paramValues.add(p.str);
					//LOG("Param: %", p);
				}
				PGresult * res = PQexecParams(
							conn.pg, query, paramValues.length, 0, paramValues.data, 0, 0, 0);
				/*if(!res)
				  throw DbError("Fatal, the sql query result is null. Error message:\n%",
				  PQerrorMessage(conn.pg));*/
				const char * errMsg = PQerrorMessage(conn.pg);
				int status = PQresultStatus(res);
				if(status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
					PQclear(res);
					throw DbError("Failed SQL command:\n%\n"
							"Status:%\nError message:\n%", query, status, errMsg);
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
				sqlite3_stmt *ppStmt = 0;
				int res = sqlite3_prepare_v2(conn.lite, query, strlen(query), &ppStmt, 0);
				if(res != SQLITE_OK){
					DbError e("Failed to prepare sqlite query:\n%\nError:\n%",
							query, sqlite3_errstr(res));
					sqlite3_finalize(ppStmt);
					throw e;
				}
				LOG("Query: %", query);
				int i = 0;
				for(const auto & p : params){
					LOG("Param: %", p);
					res = sqlite3_bind_text(ppStmt, i++, p.str, p.length, SQLITE_TRANSIENT);
					if(res != SQLITE_OK){
						DbError e("Failed to bind parameter '%' to sqlite query:\n%"
								"\nError:\n%", p, query, sqlite3_errstr(res));
						sqlite3_finalize(ppStmt);
						throw e;
					}
				}
				res = sqlite3_step(ppStmt);
				if(res != SQLITE_OK && res != SQLITE_DONE && res != SQLITE_ROW){
					DbError e("Failed to execute sqlite query:\n%\nError: %\n%",
							query, res, sqlite3_errstr(res));
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
			throw csjp::NotImplemented(EXCLI);
			break;
	}
	throw csjp::ShouldNeverReached(EXCLI);
}

SqlTableNames SqlConnection::tables()
{
	SqlTableNames tableNames;
	switch(desc.driver){
		case SqlDriver::PSql :
			{
				SqlResult result = exec("SELECT table_name FROM information_schema.tables "
						"WHERE table_schema = 'public' ORDER BY table_name;");
				for(auto & row : result)
					tableNames.add(new csjp::String(row.value(0)));
			}
			break;
		case SqlDriver::SQLite :
			{
				SqlResult result = exec("SELECT name FROM sqlite_master WHERE type='table'");
				for(auto & row : result)
					tableNames.add(new csjp::String(row.value(0)));
			}
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
	}
	return tableNames;
}

SqlColumns SqlConnection::columns(const char * tablename)
{
	SqlColumns cols;
	switch(desc.driver){
		case SqlDriver::PSql :
			{
				SqlResult result = exec("SELECT column_name "
						"FROM information_schema.columns "
						"WHERE table_schema='public' AND table_name=$1;", tablename);
				for(auto & row : result)
					cols.add(new csjp::String(row.value(0)));
			}
			break;
		case SqlDriver::SQLite :
			{
				csjp::String query;
				query.cat("PRAGMA table_info('", tablename, "')");
				SqlResult result = exec(query);
				for(auto & row : result)
					cols.add(new csjp::String(row.value(1)));
			}
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented(EXCLI);
			break;
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




SqlQuery::SqlQuery(SqlConnection & sql) :
	sql(sql),
	prepared(false)
{
	if(!sql.isOpen()) sql.open();

/*	qQuery = new QSqlQuery(sql.db);
	if(!qQuery)
		throw DbError("Failed to create QSqlQuery object");

	qQuery->setForwardOnly(true);*/
}

SqlQuery::~SqlQuery()
{
}

void SqlQuery::exec(const QString &query)
{
	(void)query;
/*	prepared = false;

	LOG("%", query);
	if(!qQuery->exec(query))
		throw DbError("The below sql query failed on connection %:\n"
				"%\nDatabase reports error: %",
				sql.desc.name,
				query,
				sql.dbErrorString());*/
}

bool SqlQuery::isPrepared()
{
//	return prepared;
	return true;
}

void SqlQuery::prepare(const QString &query)
{
	(void)query;
/*	if(!qQuery->prepare(query))
		throw DbError("Failed to prepare query:\n%error: %",
				query,
				sql.dbErrorString());*/
}

void SqlQuery::bindValue(int pos, const QVariant &v)
{
	(void)pos;
	(void)v;
/*	ENSURE(qQuery, csjp::LogicError);

	qQuery->bindValue(pos, v);*/
}

void SqlQuery::bindValue(int pos, const Text & text)
{
	(void)pos;
	(void)text;
/*	const QString & str = text;
	if(str.isNull())
		bindValue(pos, QVariant(QString("")));
	else
		bindValue(pos, QVariant(str));*/
}

void SqlQuery::bindValue(int pos, const DateTime & time)
{
	(void)pos;
	(void)time;
//	bindValue(pos, QVariant(time.isoUtcString()));
}

void SqlQuery::bindValue(int pos, const double d)
{
	(void)pos;
	(void)d;
//	bindValue(pos, QVariant(d));
}

void SqlQuery::bindValue(int pos, const int i)
{
	(void)pos;
	(void)i;
//	bindValue(pos, QVariant(i));
}

void SqlQuery::bindValue(int pos, const enum QueryStockOptions e)
{
	(void)pos;
	(void)e;
//	int i = (int)e;
//	bindValue(pos, i);
}

void SqlQuery::bindValue(int pos, const enum QueryTagOptions e)
{
	(void)pos;
	(void)e;
//	int i = (int)e;
//	bindValue(pos, i);
}

void SqlQuery::exec()
{
/*	ENSURE(qQuery, csjp::LogicError);
	ENSURE(sql.isOpen(), csjp::LogicError);

	LOG("%", queryString());
	if(!qQuery->exec())
		throw DbError("The below sql query failed on connection %:\n"
				"%\nDatabase reports error: %",
			sql.desc.name, queryString(), sql.dbErrorString());*/
}

bool SqlQuery::next()
{
/*	ENSURE(qQuery, csjp::LogicError);

	return qQuery->next();*/
	return false;
}

unsigned SqlQuery::colIndex(const QString &name)
{
	(void)name;
/*	ENSURE(qQuery, csjp::LogicError);

	int ret = qQuery->record().indexOf(name);
	if(ret < 0)
		throw DbIncompatibleTableError(
				"There is no column '%' in the result for query:\n%.",
				name, queryString());

	return ret;*/
	return 0;
}

SqlVariant SqlQuery::sqlValue(int index)
{
	(void)index;
/*	ENSURE(qQuery, csjp::LogicError);

	DBG("% : [%]",
			qQuery->record().fieldName(index),
			qQuery->value(index).toString()
			);

	return SqlVariant(qQuery->value(index));*/
	return SqlVariant((int)0);
}

QVariant SqlQuery::value(int index)
{
	(void)index;
/*	ENSURE(qQuery, csjp::LogicError);

	DBG("% : [%]",
			qQuery->record().fieldName(index),
			qQuery->value(index).toString()
			);

	return qQuery->value(index);*/
	return QVariant();
}

DateTime SqlQuery::dateTime(int index)
{
	(void)index;
	/*DateTime dt(value(index).toDateTime());
	dt.setTimeSpec(Qt::UTC);
	return dt;*/
	return DateTime();
}

Text SqlQuery::text(int index)
{
	(void)index;
	//return Text(value(index));
	return Text();
}

double SqlQuery::real(int index)
{
	(void)index;
	//return value(index).toDouble();
	return 0;
}

int SqlQuery::number(int index)
{
	(void)index;
	//return value(index).toInt();
	return 0;
}

void SqlQuery::finish()
{
	/*if(!qQuery)
		return;

	qQuery->finish();
	qQuery->clear();
	*/
}

QString SqlQuery::queryString()
{
//	ENSURE(qQuery, csjp::LogicError);

	QString str;
/*	if(qQuery->executedQuery().size())
		str += qQuery->executedQuery();
	else
		str += qQuery->lastQuery();

	QList<QVariant> list = qQuery->boundValues().values();
	for (int i = 0; i < list.size(); ++i){
		int pos = str.indexOf("?");
		ENSURE(0 <= pos, csjp::LogicError);
		QString param = "'";
		param += list.at(i).toString();
		param += "'";
		str.replace(pos, 1, param);
	}
*/
	return str;
}
