/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QVariant>

#include <QStringList>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "butler_sqlite_sql.h"

#define CONNECTION_NAME "butler_sqlite_connection"

namespace Sqlite {

#ifdef DEBUG
void listAvailableFeatures(const QSqlDatabase &db);
#endif

bool operator<(const SqlCloseListener &a, const SqlCloseListener &b)
{
	return &a < &b;
}

bool operator<(const SqlFinishListener &a, const SqlFinishListener &b)
{
	return &a < &b;
}

Sql::Sql(const QString& _path) :
	path(_path),
	lastUserErrId(Db::UNSPECIFIED_USER_ERROR),
	lastUserErr(""),
	lastErr(""),
	transactions(0)
{
}

Sql::~Sql()
{
	if(db.isOpen()){
		LOG("Closing db connection at destruction time. "
				"This can easily cause a crash.");
		close();
	}
}

void Sql::connect()
{
	ENSURE(!db.isValid(), csjp::LogicError);

	if(!QSqlDatabase::contains(CONNECTION_NAME)){
		db = QSqlDatabase::addDatabase(
				"QSQLITE", CONNECTION_NAME);
		if(db.lastError().isValid())
			throw DbError("Failed to add SQLITE database driver: %s",
					C_STR(dbErrorString()));
	} else {
		db = QSqlDatabase::database(CONNECTION_NAME, false);
		if(db.lastError().isValid())
			throw DbError("Failed to get database connection named '%s'.\nError: %s",
					CONNECTION_NAME,
					C_STR(dbErrorString()));
	}
	ENSURE(db.isValid(), csjp::LogicError);
	
	db.setDatabaseName(path);
	if(db.lastError().isValid())
		throw DbError("Failed to set database name to %s.\nError: %s",
				C_STR(path),
				C_STR(dbErrorString()));

#ifdef DEBUG
	listAvailableFeatures(db);
#endif
}

void Sql::open()
{
	if(db.isOpen()) return;

//	ENSURE(db.isValid(), csjp::LogicError);

	db.open();
	if(db.lastError().isValid())
		throw DbError("Failed to open database '%s'.\nError: %s",
				C_STR(db.databaseName()),
				C_STR(dbErrorString()));

	ENSURE(db.isOpen(), csjp::LogicError);

	/* Lets check if the sqlite on system is compatible with us. */

	/* Lets turn on reference constraits */
	QSqlQuery q = db.exec("PRAGMA foreign_keys = ON");
	q = db.exec("PRAGMA foreign_keys");
	ENSURE(q.isActive(), csjp::LogicError);
	ENSURE(q.isSelect(), csjp::LogicError);

	q.next();

	DBG("Reference constraint support: %s",
			(q.value(0).toInt()) ? "yes" : "no");

	if(q.value(0).toInt() != 1){
		db.close();
		throw DbError("Reference constraits could not turned "
			"on or not supported at all.");
	}
}

void Sql::close()
{
	if(!db.isOpen()) return;

	ENSURE(db.isOpen(), csjp::LogicError);

	/* Lets notify all listener queries that db is about to be closed. */
	notifySqlCloseListeners();

	db.close();
	if(db.lastError().isValid())
		throw DbError("Failed to close database '%s'.\nError: %s",
				C_STR(db.databaseName()),
				C_STR(dbErrorString()));

	ENSURE(!db.isOpen(), csjp::LogicError);
	
	db = QSqlDatabase();
	QSqlDatabase::removeDatabase(CONNECTION_NAME);
}

enum Db::UserError Sql::lastUserErrorId()
{
	return lastUserErrId;
}

const QString& Sql::lastUserError()
{
	return lastUserErr;
}
		
const QString& Sql::lastError()
{
	return lastErr;
}

/*
 *	Private members
 */

/*Sql::Sql()
{
}
*/

QSqlQuery* Sql::createQuery()
{
	QSqlQuery * q = new QSqlQuery(db);
	if(!q)
		throw DbError("Failed to create QSqlQuery object");
	return q;
}

bool Sql::exec(const QString &cmd)
{
	bool ret = true;

	db.exec(cmd);
	ret = reportSqlError();

	DBG("SQL: %s", qPrintable(cmd));

	return ret;
}

QSqlRecord Sql::record(const QString &tablename) const
{
	return db.record(tablename);
}

QStringList Sql::tables() const
{
	return db.tables();
}

bool Sql::isOpen()
{
	return db.isOpen();
}

bool Sql::transaction()
{
	ENSURE(0 <= transactions, csjp::LogicError);

	bool ret = true;

	if(transactions == 0){
		DBG("BEGIN TRANSACTION");
		ret = db.transaction();
		ret = reportSqlError() && ret;
	}

	if(ret)
		transactions++;

	return ret;
}

bool Sql::commit()
{
	ENSURE(0 < transactions, csjp::LogicError);

	bool ret = true;

	if(transactions == 1){
		DBG("COMMIT TRANSACTION");
		ret = db.commit();
		ret = reportSqlError() && ret;
	}

	if(ret)
		transactions--;

	return ret;
}

bool Sql::rollback()
{
	ENSURE(0 < transactions, csjp::LogicError);

	bool ret = true;

	if(transactions == 1){
		DBG("ROLLBACK TRANSACTION");
#if 0
		notifySqlFinishListeners();
#endif
		ret = db.rollback();
		if(!ret)
			LOG("Serious error happened: Rolling back database changes failed. ");
	}

	if(ret)
		transactions--;

	return ret;
}

/* returns false on error */
bool Sql::reportSqlError()
{
	bool ret = true;
	if(db.lastError().isValid()){
		lastErr = db.lastError().text();
		lastUserErr = "";
		lastUserErrId = Db::UNSPECIFIED_USER_ERROR;
		LOG("QSqlDatabase error: %s", qPrintable(lastErr));
		ret = false;
	}
	return ret;
}

QString Sql::dbErrorString()
{
	return db.lastError().text();
}

void Sql::addSqlCloseListener(SqlCloseListener &l)
{
	sqlCloseListeners.add(l);
}

void Sql::removeSqlCloseListener(SqlCloseListener &l)
{
	sqlCloseListeners.remove(l);
}
#if 0	
void Sql::notifySqlFinishListeners()
{
	unsigned s,i;
	s = sqlFinishListeners.size();
	for(i=0; i<s; i++)
		sqlFinishListeners.queryAt(i).sqlFinishNotification();
}
#endif			
void Sql::notifySqlCloseListeners()
{
	unsigned s,i;
	s = sqlCloseListeners.size();
	for(i=0; i<s; i++)
		sqlCloseListeners.queryAt(i).sqlCloseNotification();
}

#ifdef DEBUG
void listAvailableFeatures(const QSqlDatabase &db)
{
	static bool featuresListed = false;

	if(featuresListed)
		return;

	DBG("Available drivers: %s",
			qPrintable((QSqlDatabase::drivers().join(", "))));

	QSqlDriver *drv = db.driver();
	ENSURE(drv != NULL, csjp::LogicError);
	QString format = "QSqlDriver features for connection %s :";
	int i;
	for(i=0; i<14; i++)
		format += "\n\t%-25s%3d";

	DBG(qPrintable(format),
			CONNECTION_NAME,
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

}
