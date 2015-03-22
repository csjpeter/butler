/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <csjp_object.h>
#include <csjp_owner_container.h>

#include <butler_macros.h>

#include <QStringList>

#include "butler_sql_connection.h"
/*
int SqlResult::numOfCols() const
{
	switch(driver){
		case SqlDriver::PSql :
			return PQnfields(res.pg);
		case SqlDriver::SQLite :
			break;
		case SqlDriver::MySQL :
			break;
	}
}
*/
char * SqlResult::value(int col) const
{
	switch(driver){
		case SqlDriver::PSql :
			return PQgetvalue(res.pg, row, col);
		case SqlDriver::SQLite :
			return 0;
		case SqlDriver::MySQL :
			return 0;
	}
	Throw(csjp::ShouldNeverReached);
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
			sqlite3_step(res.lite);
			row++;
			return false;
		case SqlDriver::MySQL :
			return false;
	}
	Throw(csjp::ShouldNeverReached);
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
		LOG("%s", e.what());
	} catch (std::exception & e) {
		LOG("%s", e.what());
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
						throw csjp::LogicError("Unexpected connection state %d.", state);
				}
			}
			break;
		case SqlDriver::SQLite :
			return conn.lite != 0;
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented();
			break;
	}
	return false;
}

void SqlConnection::open()
{
	switch(desc.driver){
		case SqlDriver::PSql :
			{
				csjp::String str, connStr;
/*				str.cat("host='",desc.host, "' port='", desc.port, "'",
						" dbname='", desc.databaseName, "'",
						" username='", desc.username, "'",
						" sslmode='allow'");
				connStr.cat(str, " password='", desc.password, "'");*/
				str.cat("postgres://",desc.username,"@",
						desc.host,":",desc.port,"/",desc.databaseName);
				connStr.cat("postgres://",desc.username,":",desc.password,"@",
						desc.host,":",desc.port,"/",desc.databaseName);
				conn.pg = PQconnectdb(connStr.str);
				if(PQstatus(conn.pg) != CONNECTION_OK){
					PQfinish(conn.pg);
					throw DbError("Failed to connect to postgresql with connection string:\n"
							"%s\n\nError:\n%s", str.str, PQerrorMessage(conn.pg));
				}
			}
			break;
		case SqlDriver::SQLite :
			{
				int res = sqlite3_open_v2(desc.databaseName.str, &(conn.lite), 0, 0);
				if(res != SQLITE_OK || conn.lite == 0)
					throw DbError("Failed to open sqlite database/file '%s'\n",
							desc.databaseName.str);
				/*q = db.exec("PRAGMA foreign_keys = ON");
				q = db.exec("PRAGMA foreign_keys");
        		if(q.value(0).toInt() != 1){
					db.close();
					throw DbError("Reference constraits could not turned "
							"on or not supported at all.");
				}*/
			}
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented();
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
					throw DbError("Failed to close sqlite database/file '%s'\n",
							desc.databaseName.str);
				conn.lite = 0;
			}
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented();
			break;
	}
	ENSURE(!isOpen(), csjp::LogicError);
}

SqlResult SqlConnection::exec(const char * query, csjp::unint len)
{
	if(!isOpen()) open();
	switch(desc.driver){
		case SqlDriver::PSql :
			{
				PGresult * res = PQexec(conn.pg, query);
				/*if(!res)
					throw DbError("Fatal, the sql query result is null. Error message:\n%s",
							PQerrorMessage(conn.pg));*/
				if(PQresultStatus(res) != PGRES_COMMAND_OK) {
					PQclear(res);
					throw DbError("Failed SQL command:\n%s\n\nError message:\n%s",
							query, PQerrorMessage(conn.pg));
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
				int res = sqlite3_prepare_v2(conn.lite, query, len, &ppStmt, 0);
				if(res != SQLITE_OK || errmsg != 0){
					DbError e("Failed to prepare sqlite query:\n%s\n\nError:\n%s",
							query, errmsg);
					sqlite3_free(errmsg);
					sqlite3_finalize(ppStmt);
					throw e;
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

#if 0
template<typename... Args>
SqlResult SqlConnection::exec(const char * query, const Args & ... args)
{
	if(!isOpen()) open();
	switch(desc.driver){
		case SqlDriver::PSql :
			{
				/*PGresult *PQexecParams(PGconn *conn,
						const char *command,
						int nParams,
						const Oid *paramTypes,
						const char * const *paramValues,
						const int *paramLengths,
						const int *paramFormats,
						int resultFormat);*/
				PGresult * res = PQexec(conn.pg, query);
				/*if(!res)
					throw DbError("Fatal, the sql query result is null. Error message:\n%s",
							PQerrorMessage(conn.pg));*/
				if(PQresultStatus(res) != PGRES_COMMAND_OK) {
					PQclear(res);
					throw DbError("Failed SQL command:\n%s\n\nError message:\n%s",
							query, PQerrorMessage(conn.pg));
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
			throw csjp::NotImplemented();
			break;
		case SqlDriver::MySQL :
			throw csjp::NotImplemented();
			break;
	}
	Throw(csjp::ShouldNeverReached);
}
#endif

SqlColumns SqlConnection::columns(const QString &tablename)
{
	(void)tablename;
	SqlColumns cols;
	/*QSqlRecord rec = db.record(tablename);
	int c = rec.count();
	for(int i = 0; i < c; i++)
		cols.add(new csjp::String(C_STR(rec.field(i).name())));*/
	return cols;
}

const SqlTableNames & SqlConnection::tables()
{
	if(!tableNames.size()){
		SqlResult result = exec(	"SELECT table_name FROM information_schema.tables "
				"WHERE table_schema = 'public' ORDER BY table_name;");
/*		for(const auto & row : result)
			tableNames.add(new csjp::String(row.value()));*/
	}
	return tableNames;
}
/*
QString SqlConnection::dbErrorString()
{
	QString str;
	switch(desc.driver){
		case SqlDriver::PSql :
			str = PQerrorMessage(conn.pg);
			break;
		default:
			str = db.lastError().text();
	}
	return str;
}
*/

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

	LOG("%s", C_STR(query));
	if(!qQuery->exec(query))
		throw DbError("The below sql query failed on connection %s:\n"
				"%s\nDatabase reports error: %s",
				C_STR(sql.desc.name),
				C_STR(query),
				C_STR(sql.dbErrorString()));*/
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
		throw DbError("Failed to prepare query:\n%serror: %s",
				C_STR(query),
				C_STR(sql.dbErrorString()));*/
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

	LOG("%s", C_STR(queryString()));
	if(!qQuery->exec())
		throw DbError("The below sql query failed on connection %s:\n"
				"%s\nDatabase reports error: %s",
			C_STR(sql.desc.name), C_STR(queryString()), C_STR(sql.dbErrorString()));*/
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
				"There is no column '%s' in the result for query:\n%s.",
				C_STR(name), C_STR(queryString()));

	return ret;*/
	return 0;
}

SqlVariant SqlQuery::sqlValue(int index)
{
	(void)index;
/*	ENSURE(qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			C_STR(qQuery->record().fieldName(index)),
			C_STR(qQuery->value(index).toString())
			);

	return SqlVariant(qQuery->value(index));*/
	return SqlVariant((int)0);
}

QVariant SqlQuery::value(int index)
{
	(void)index;
/*	ENSURE(qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			C_STR(qQuery->record().fieldName(index)),
			C_STR(qQuery->value(index).toString())
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
