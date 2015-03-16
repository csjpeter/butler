/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#define DEBUG
#undef DEBUG

#include <csjp_object.h>
#include <csjp_owner_container.h>

#include <butler_macros.h>

#include <QStringList>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include "butler_sql_connection.h"

#ifdef DEBUG
void listAvailableFeatures(const QSqlDatabase & db, const DatabaseDescriptor & dbDesc);
#endif

typedef struct SqlConnectionPrivate
{
	SqlConnectionPrivate() :
		transactions(0)
	{}

	QSqlDatabase db;
	unsigned transactions;
	mutable SqlTableNames tables;
} SqlConnectionPrivate;

SqlConnection::SqlConnection(const DatabaseDescriptor & dbDesc) :
	dbDesc(dbDesc),
	priv(NULL)
{
	csjp::Object<SqlConnectionPrivate> p(new SqlConnectionPrivate());
	priv = p.ptr;

	ENSURE(!priv->db.isValid(), csjp::LogicError);

#ifdef DEBUG
	DBG("Available drivers: %s", C_STR((QSqlDatabase::drivers().join(", "))));
#endif

//	if(!QSqlDatabase::contains(dbDesc.name)){
		priv->db = QSqlDatabase::addDatabase(dbDesc.driver, dbDesc.name);
		if(priv->db.lastError().isValid())
			throw DbError("Failed to add SQLITE database driver: %s",
					C_STR(dbErrorString()));
/*	} else {
		// At once tries to open the connection with old details
		priv->db = QSqlDatabase::database(dbDesc.name, false);
		if(priv->db.lastError().isValid())
			throw DbError("Failed to get database connection named '%s'.\nError: %s",
					C_STR(dbDesc.name), C_STR(dbErrorString()));
	}*/
	ENSURE(priv->db.isValid(), csjp::LogicError);

	open();
	p.ptr = NULL;
}

SqlConnection::~SqlConnection()
{
	try {
		close();
		/* FIXME db should be destroyed before this call
		   QSqlDatabase::removeDatabase(dbDesc.name); */
	} catch (csjp::Exception & e) {
		LOG("%s", e.what());
	} catch (std::exception & e) {
		LOG("%s", e.what());
	}
	delete priv;
}

void SqlConnection::open()
{
	if(priv->db.isOpen()) return;

	ENSURE(priv->db.isValid(), csjp::LogicError);

	priv->db.setDatabaseName(dbDesc.databaseName);
	if(priv->db.lastError().isValid())
		throw DbError("Failed to set database name to %s.\nError: %s",
				C_STR(dbDesc.databaseName),
				C_STR(dbErrorString()));

	if(dbDesc.driver != "QSQLITE"){
		priv->db.setHostName(dbDesc.host);
		priv->db.setUserName(dbDesc.username);
		priv->db.setPassword(dbDesc.password);
		priv->db.setPort(dbDesc.port);
		priv->db.setConnectOptions("sslmode=require");
	} else {
		priv->db.setPort(-1);
	}

#ifdef DEBUG
	listAvailableFeatures(priv->db, dbDesc);
#endif

	priv->db.open();
	if(priv->db.lastError().isValid())
		throw DbConnectError("Failed to connect to database '%s'.\nError: %s",
				C_STR(priv->db.databaseName()),
				C_STR(dbErrorString()));
	LOG("Connected to database '%s' on host '%s:%d' as user '%s'.", C_STR(dbDesc.databaseName),
			C_STR(dbDesc.host), dbDesc.port, C_STR(dbDesc.username));

	ENSURE(priv->db.isOpen(), csjp::LogicError);

	if(dbDesc.driver == "QSQLITE"){
		/* Lets check if the sqlite on system is compatible with us. */
		/* Lets turn on reference constraits */
		QSqlQuery q = priv->db.exec("PRAGMA foreign_keys = ON");
		q = priv->db.exec("PRAGMA foreign_keys");
		ENSURE(q.isActive(), csjp::LogicError);
		ENSURE(q.isSelect(), csjp::LogicError);

		q.next();

		DBG("Reference constraint support: %s", (q.value(0).toInt()) ? "yes" : "no");

		if(q.value(0).toInt() != 1){
			priv->db.close();
			throw DbError("Reference constraits could not turned "
					"on or not supported at all.");
		}
	}
}

void SqlConnection::close()
{
	if(!priv->db.isOpen()) return;

	ENSURE(priv->db.isOpen(), csjp::LogicError);

	priv->db.close();
	if(priv->db.lastError().isValid())
		throw DbError("Failed to close database '%s'.\nError: %s",
				C_STR(priv->db.databaseName()),
				C_STR(dbErrorString()));

	ENSURE(!priv->db.isOpen(), csjp::LogicError);
}

/*
 *	Private members
 */

void SqlConnection::exec(const QString &query)
{
	open();

	QSqlQuery qQuery(priv->db);
	qQuery.setForwardOnly(true);

	DBG("%s", C_STR(query));
	if(!qQuery.exec(query))
		throw DbError("The below sql query failed:\n%s\nDatabase reports error: %s",
				C_STR(query),
				C_STR(dbErrorString()));
}

SqlColumns SqlConnection::columns(const QString &tablename) const
{
	QSqlRecord rec = priv->db.record(tablename);
	SqlColumns cols;
	int c = rec.count();
	for(int i = 0; i < c; i++)
		cols.add(new csjp::String(C_STR(rec.field(i).name())));
	return cols;
}

const SqlTableNames & SqlConnection::tables() const
{
	if(!priv->tables.size()){
		QStringList list(priv->db.tables());
		for(int i = 0; i < list.size(); i++)
			priv->tables.add(new csjp::String(C_STR(list[i])));
	}
	return priv->tables;
}

bool SqlConnection::isOpen()
{
	return priv->db.isOpen();
}

QString SqlConnection::dbErrorString()
{
	return priv->db.lastError().text();
}

#ifdef DEBUG
void listAvailableFeatures(const QSqlDatabase & db, const DatabaseDescriptor & dbDesc)
{
	static bool featuresListed = false;

	if(featuresListed)
		return;

	QSqlDriver *drv = db.driver();
	ENSURE(drv != NULL, csjp::LogicError);
	QString format = "QSqlDriver features for connection %s :";
	int i;
	for(i=0; i<14; i++)
		format += "\n\t%-25s%3d";

	DBG(C_STR(format),
			C_STR(dbDesc.name),
			"Transactions",
			drv->hasFeature(QSqlDriver::Transactions),
			"QuerySize",
			drv->hasFeature(QSqlDriver::QuerySize),
			"BLOB",
			drv->hasFeature(QSqlDriver::BLOB),
			"Unicode",
			drv->hasFeature(QSqlDriver::Unicode),
			"PreparedQueries",
			drv->hasFeature(QSqlDriver::PreparedQueries),
			"NamedPlaceholders",
			drv->hasFeature(QSqlDriver::NamedPlaceholders),
			"PositionalPlaceholders",
			drv->hasFeature(QSqlDriver::PositionalPlaceholders),
			"LastInsertId",
			drv->hasFeature(QSqlDriver::LastInsertId),
			"BatchOperations",
			drv->hasFeature(QSqlDriver::BatchOperations),
			"SimpleLocking",
			drv->hasFeature(QSqlDriver::SimpleLocking),
			"LowPrecisionNumbers",
			drv->hasFeature(QSqlDriver::LowPrecisionNumbers),
			"EventNotifications",
			drv->hasFeature(QSqlDriver::EventNotifications),
			"FinishQuery",
			drv->hasFeature(QSqlDriver::FinishQuery),
			"MultipleResultSets",
			drv->hasFeature(QSqlDriver::MultipleResultSets)
		);

	featuresListed = true;
}
#endif

SqlTransaction::SqlTransaction(SqlConnection & sql) :
	sql(sql),
	committed(false)
{
	sql.open();

	if(sql.priv->transactions == 0){
		DBG("BEGIN TRANSACTION");
		sql.priv->db.transaction();
		if(sql.priv->db.lastError().isValid())
			throw DbError("Failed begin transcation.\nError: %s",
					C_STR(sql.dbErrorString()));
	}

	sql.priv->transactions++;
}

SqlTransaction::~SqlTransaction()
{
	if(committed)
		return;

	if(sql.priv->transactions == 1){
		DBG("ROLLBACK TRANSACTION");
		sql.priv->db.rollback();
		if(sql.priv->db.lastError().isValid())
			LOG("Failed to rollback transcation.\nError: %s", C_STR(sql.dbErrorString()));
			/* FIXME test this */
/*			throw DbError("Failed to rollback transcation.\nError: %s",
					C_STR(sql.dbErrorString()));
*/
	}

	sql.priv->transactions--;
}

void SqlTransaction::commit()
{
	if(committed)
		return;
	committed = true;

	if(sql.priv->transactions == 1){
		DBG("COMMIT TRANSACTION");
		sql.priv->db.commit();
		if(sql.priv->db.lastError().isValid())
			throw DbError("Failed to commit transcation.\nError: %s",
					C_STR(sql.dbErrorString()));
	}

	sql.priv->transactions--;
}




typedef struct SqlQueryPrivate
{
	SqlQueryPrivate() :
		qQuery(NULL),
		prepared(false)
	{}

	~SqlQueryPrivate()
	{
		delete qQuery;
	}

	QSqlQuery * qQuery;
	bool prepared;
} SqlQueryPrivate;

SqlQuery::SqlQuery(SqlConnection & sql) :
	sql(sql)
{
	csjp::Object<SqlQueryPrivate> p(new SqlQueryPrivate());
	priv = p.ptr;

	sql.open();

	priv->qQuery = new QSqlQuery(sql.priv->db);
	if(!priv->qQuery)
		throw DbError("Failed to create QSqlQuery object");

	priv->qQuery->setForwardOnly(true);

	p.ptr = NULL;
}

SqlQuery::~SqlQuery()
{
	delete priv;
}

void SqlQuery::exec(const QString &query)
{
	priv->prepared = false;

	LOG("%s", C_STR(query));
	if(!priv->qQuery->exec(query))
		throw DbError("The below sql query failed on connection %s:\n"
				"%s\nDatabase reports error: %s",
				C_STR(sql.dbDesc.name),
				C_STR(query),
				C_STR(sql.dbErrorString()));
}

bool SqlQuery::isPrepared()
{
	return priv->prepared;
}

void SqlQuery::prepare(const QString &query)
{
	if(!priv->qQuery->prepare(query))
		throw DbError("Failed to prepare query:\n%serror: %s",
				C_STR(query),
				C_STR(sql.dbErrorString()));
}

void SqlQuery::bindValue(int pos, const QVariant &v)
{
	ENSURE(priv->qQuery, csjp::LogicError);

	priv->qQuery->bindValue(pos, v);
}

void SqlQuery::bindValue(int pos, const Text & text)
{
	const QString & str = text;
	if(str.isNull())
		bindValue(pos, QVariant(QString("")));
	else
		bindValue(pos, QVariant(str));
}

void SqlQuery::bindValue(int pos, const DateTime & time)
{
	bindValue(pos, QVariant(time.isoUtcString()));
}

void SqlQuery::bindValue(int pos, const double d)
{
	bindValue(pos, QVariant(d));
}

void SqlQuery::bindValue(int pos, const int i)
{
	bindValue(pos, QVariant(i));
}

void SqlQuery::bindValue(int pos, const enum QueryStockOptions e)
{
	int i = (int)e;
	bindValue(pos, i);
}

void SqlQuery::bindValue(int pos, const enum QueryTagOptions e)
{
	int i = (int)e;
	bindValue(pos, i);
}

void SqlQuery::exec()
{
	ENSURE(priv->qQuery, csjp::LogicError);
	ENSURE(sql.isOpen(), csjp::LogicError);

	LOG("%s", C_STR(queryString()));
	if(!priv->qQuery->exec())
		throw DbError("The below sql query failed on connection %s:\n"
				"%s\nDatabase reports error: %s",
			C_STR(sql.dbDesc.name), C_STR(queryString()), C_STR(sql.dbErrorString()));
}

bool SqlQuery::next()
{
	ENSURE(priv->qQuery, csjp::LogicError);

	return priv->qQuery->next();
}

unsigned SqlQuery::colIndex(const QString &name)
{
	ENSURE(priv->qQuery, csjp::LogicError);

	int ret = priv->qQuery->record().indexOf(name);
	if(ret < 0)
		throw DbIncompatibleTableError(
				"There is no column '%s' in the result for query:\n%s.",
				C_STR(name), C_STR(queryString()));

	return ret;
}

SqlVariant SqlQuery::sqlValue(int index)
{
	ENSURE(priv->qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			C_STR(priv->qQuery->record().fieldName(index)),
			C_STR(priv->qQuery->value(index).toString())
			);

	return SqlVariant(priv->qQuery->value(index));
}

QVariant SqlQuery::value(int index)
{
	ENSURE(priv->qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			C_STR(priv->qQuery->record().fieldName(index)),
			C_STR(priv->qQuery->value(index).toString())
			);

	return priv->qQuery->value(index);
}

DateTime SqlQuery::dateTime(int index)
{
	DateTime dt(value(index).toDateTime());
	dt.setTimeSpec(Qt::UTC);
	return dt;
}

Text SqlQuery::text(int index)
{
	return Text(value(index));
}

double SqlQuery::real(int index)
{
	return value(index).toDouble();
}

int SqlQuery::number(int index)
{
	return value(index).toInt();
}

void SqlQuery::finish()
{
	if(!priv->qQuery)
		return;

	priv->qQuery->finish();
	priv->qQuery->clear();
}

QString SqlQuery::queryString()
{
	ENSURE(priv->qQuery, csjp::LogicError);

	QString str;
	if(priv->qQuery->executedQuery().size())
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

	return str;
}
