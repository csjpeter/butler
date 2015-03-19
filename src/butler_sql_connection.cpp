/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <postgresql/libpq-fe.h>
#include <sqlite3.h>
//#include <sqlite3ext.h>

#include <csjp_object.h>
#include <csjp_owner_container.h>

#include <butler_macros.h>

#include <QStringList>

#include "butler_sql_connection.h"

enum class SqlConnectionType
{
	PSql,
	SQLite
};

typedef struct SqlConnectionPrivate
{
	SqlConnectionPrivate(SqlConnectionType type) :
		type(type)
	{
		transactions = 0;

		switch(type){
			case SqlConnectionType::PSql :
				conn.pg = 0;
				break;
			case SqlConnectionType::SQLite :
				conn.lite = 0;
				break;
		}
	}

	const SqlConnectionType type;

	union ConnectionPtr{
		PGconn * pg;
		sqlite3 * lite;
	} conn;
	unsigned transactions;
	mutable SqlTableNames tables;
} SqlConnectionPrivate;

SqlConnection::SqlConnection(const DatabaseDescriptor & dbDesc) :
	dbDesc(dbDesc),
	priv(NULL),
	desc(dbDesc)
{
	SqlConnectionType type;
	if(dbDesc.driver == "SQLITE")
		type = SqlConnectionType::SQLite;
	else if(dbDesc.driver == "PSQL")
		type = SqlConnectionType::PSql;
	else
		throw new csjp::InvalidArgument("Unsupported SQL driver '%s' requested.",dbDesc.driver.str);

	csjp::Object<SqlConnectionPrivate> p(new SqlConnectionPrivate(type));
	priv = p.ptr;
	open();
	p.ptr = NULL;
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
	delete priv;
}

bool SqlConnection::isOpen()
{
	switch(priv->type){
		case SqlConnectionType::PSql :
			return priv->conn.pg != 0;
			break;
		case SqlConnectionType::SQLite :
			return priv->conn.lite != 0;
			break;
	}
	return false;
}

void SqlConnection::open()
{
	if(isOpen()) return;

	switch(priv->type){
		case SqlConnectionType::PSql :
			{
				csjp::String str, connStr;
/*				str.cat("host='",dbDesc.host, "' port='", dbDesc.port, "'",
						" dbname='", dbDesc.databaseName, "'",
						" username='", dbDesc.username, "'",
						" sslmode='allow'");
				connStr.cat(str, " password='", dbDesc.password, "'");*/
				str.cat("postgres://",dbDesc.username,"@",
						dbDesc.host,":",dbDesc.port,"/",dbDesc.databaseName);
				connStr.cat("postgres://",dbDesc.username,":",dbDesc.password,"@",
						dbDesc.host,":",dbDesc.port,"/",dbDesc.databaseName);
				priv->conn.pg = PQconnectdb(connStr.str);
				if(PQstatus(priv->conn.pg) != CONNECTION_OK){
					PQfinish(priv->conn.pg);
					throw DbError("Failed to connect to postgresql with connection string:\n"
							"%s\n\nError:\n%s", str.str, PQerrorMessage(priv->conn.pg));
				}
			}
		break;
		case SqlConnectionType::SQLite :
			{
				int res = sqlite3_open(dbDesc.databaseName.str, &(priv->conn.lite));
				if(res < 0 || priv->conn.lite == 0)
					throw DbError("Failed to open sqlite database/file '%s'\n",
							dbDesc.databaseName.str);
				/*q = priv->db.exec("PRAGMA foreign_keys = ON");
				q = priv->db.exec("PRAGMA foreign_keys");
        		if(q.value(0).toInt() != 1){
					priv->db.close();
					throw DbError("Reference constraits could not turned "
							"on or not supported at all.");
				}*/
			}
		break;
	}
	ENSURE(isOpen(), csjp::LogicError);
}

void SqlConnection::close()
{
	if(!isOpen()) return;

	switch(priv->type){
		case SqlConnectionType::PSql :
			PQfinish(priv->conn.pg);
			priv->conn.pg = 0;
			break;
		case SqlConnectionType::SQLite :
			{
				int res = sqlite3_close(priv->conn.lite);
				if(res < 0)
					throw DbError("Failed to close sqlite database/file '%s'\n",
							dbDesc.databaseName.str);
				priv->conn.lite = 0;
			}
			break;
	}
	ENSURE(!isOpen(), csjp::LogicError);
}

void SqlConnection::exec(const char * query)
{
	open();
	switch(priv->type){
		case SqlConnectionType::PSql :
			{
				PGresult * res = PQexec(priv->conn.pg, query);
				if(PQresultStatus(res) != PGRES_COMMAND_OK) {
					PQclear(res);
					throw DbError("Failed SQL command:\n%s\n\nError message:\n%s",
							query, PQerrorMessage(priv->conn.pg));
				}
				PQclear(res);
			}
			break;
		case SqlConnectionType::SQLite :
			{
				char * errmsg = 0;
				int res = sqlite3_exec(priv->conn.lite, query, 0, 0, &errmsg);
				if(res < 0 || errmsg != 0){
					DbError e("Failed to execute sqlite query:\n%s\n\nError:\n%s",
							query, errmsg);
					sqlite3_free(errmsg);
					throw e;
				}
			}
			break;
	}
}

SqlColumns SqlConnection::columns(const QString &tablename) const
{
	(void)tablename;
	SqlColumns cols;
	/*QSqlRecord rec = priv->db.record(tablename);
	int c = rec.count();
	for(int i = 0; i < c; i++)
		cols.add(new csjp::String(C_STR(rec.field(i).name())));*/
	return cols;
}

const SqlTableNames & SqlConnection::tables() const
{
/*	if(!priv->tables.size()){
		QStringList list(priv->db.tables());
		for(int i = 0; i < list.size(); i++)
			priv->tables.add(new csjp::String(C_STR(list[i])));
	}*/
	return priv->tables;
}
/*
QString SqlConnection::dbErrorString()
{
	QString str;
	switch(priv->type){
		case SqlConnectionType::PSql :
			str = PQerrorMessage(priv->conn.pg);
			break;
		default:
			str = priv->db.lastError().text();
	}
	return str;
}
*/

SqlTransaction::SqlTransaction(SqlConnection & sql) :
	sql(sql),
	committed(false)
{
	if(sql.priv->transactions == 0)
		sql.exec("BEGIN TRANSACTION");
	sql.priv->transactions++;
}

SqlTransaction::~SqlTransaction()
{
	if(committed)
		return;
	if(sql.priv->transactions == 1)
		sql.exec("ROLLBACK");
	sql.priv->transactions--;
}

void SqlTransaction::commit()
{
	if(committed)
		return;
	committed = true;
	if(sql.priv->transactions == 1)
		sql.exec("COMMIT");
	sql.priv->transactions--;
}




typedef struct SqlQueryPrivate
{
	SqlQueryPrivate() :
		//qQuery(NULL),
		prepared(false)
	{}

	~SqlQueryPrivate()
	{
		//delete qQuery;
	}

	//QSqlQuery * qQuery;
	bool prepared;
} SqlQueryPrivate;

SqlQuery::SqlQuery(SqlConnection & sql) :
	sql(sql)
{
	csjp::Object<SqlQueryPrivate> p(new SqlQueryPrivate());
	priv = p.ptr;

	sql.open();

/*	priv->qQuery = new QSqlQuery(sql.priv->db);
	if(!priv->qQuery)
		throw DbError("Failed to create QSqlQuery object");

	priv->qQuery->setForwardOnly(true);*/

	p.ptr = NULL;
}

SqlQuery::~SqlQuery()
{
	delete priv;
}

void SqlQuery::exec(const QString &query)
{
	(void)query;
/*	priv->prepared = false;

	LOG("%s", C_STR(query));
	if(!priv->qQuery->exec(query))
		throw DbError("The below sql query failed on connection %s:\n"
				"%s\nDatabase reports error: %s",
				C_STR(sql.dbDesc.name),
				C_STR(query),
				C_STR(sql.dbErrorString()));*/
}

bool SqlQuery::isPrepared()
{
//	return priv->prepared;
	return true;
}

void SqlQuery::prepare(const QString &query)
{
	(void)query;
/*	if(!priv->qQuery->prepare(query))
		throw DbError("Failed to prepare query:\n%serror: %s",
				C_STR(query),
				C_STR(sql.dbErrorString()));*/
}

void SqlQuery::bindValue(int pos, const QVariant &v)
{
	(void)pos;
	(void)v;
/*	ENSURE(priv->qQuery, csjp::LogicError);

	priv->qQuery->bindValue(pos, v);*/
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
/*	ENSURE(priv->qQuery, csjp::LogicError);
	ENSURE(sql.isOpen(), csjp::LogicError);

	LOG("%s", C_STR(queryString()));
	if(!priv->qQuery->exec())
		throw DbError("The below sql query failed on connection %s:\n"
				"%s\nDatabase reports error: %s",
			C_STR(sql.dbDesc.name), C_STR(queryString()), C_STR(sql.dbErrorString()));*/
}

bool SqlQuery::next()
{
/*	ENSURE(priv->qQuery, csjp::LogicError);

	return priv->qQuery->next();*/
	return false;
}

unsigned SqlQuery::colIndex(const QString &name)
{
	(void)name;
/*	ENSURE(priv->qQuery, csjp::LogicError);

	int ret = priv->qQuery->record().indexOf(name);
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
/*	ENSURE(priv->qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			C_STR(priv->qQuery->record().fieldName(index)),
			C_STR(priv->qQuery->value(index).toString())
			);

	return SqlVariant(priv->qQuery->value(index));*/
	return SqlVariant((int)0);
}

QVariant SqlQuery::value(int index)
{
	(void)index;
/*	ENSURE(priv->qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			C_STR(priv->qQuery->record().fieldName(index)),
			C_STR(priv->qQuery->value(index).toString())
			);

	return priv->qQuery->value(index);*/
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
	/*if(!priv->qQuery)
		return;

	priv->qQuery->finish();
	priv->qQuery->clear();
	*/
}

QString SqlQuery::queryString()
{
//	ENSURE(priv->qQuery, csjp::LogicError);

	QString str;
/*	if(priv->qQuery->executedQuery().size())
		str += priv->qQuery->executedQuery();
	else
		str += priv->qQuery->lastQuery();

	QList<QVariant> list = priv->qQuery->boundValues().values();
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
