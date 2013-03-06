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
	lastErrId(Db::NoError),
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
	bool ret;

	if(!QSqlDatabase::contains(CONNECTION_NAME)){
		db = QSqlDatabase::addDatabase(
				"QSQLITE", CONNECTION_NAME);
		ret = reportSqlError();
	} else {
		db = QSqlDatabase::database(CONNECTION_NAME, false);
		ret = reportSqlError();
	}
	ENSURE(ret == db.isValid(), csjp::LogicError);
	
	if(ret){
		db.setDatabaseName(path);
		ret = reportSqlError();
	}

#ifdef DEBUG
	if(ret)
		listAvailableFeatures(db);
#endif

	return ret;
}

void Sql::open()
{
	ENSURE(db.isValid(), csjp::LogicError);

	db.open();
	reportSqlError();
	ENSURE(ret == db.isOpen(), csjp::LogicError);

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
	if(!db.isOpen()) /* I dont understand why this can happen, but lets count with it. */
		return true;

	ENSURE(db.isOpen(), csjp::LogicError);
	bool ret;

	/* Lets notify all listener queries that db is about to be closed. */
	notifySqlCloseListeners();

	db.close();
	ret = reportSqlError();
	ENSURE(ret = !db.isOpen(), csjp::LogicError);
	
	db = QSqlDatabase();
	QSqlDatabase::removeDatabase(CONNECTION_NAME);
	
	return ret;
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
	return new QSqlQuery(db);
}

void Sql::exec(const QString &cmd)
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
#if 0
void Sql::transaction()
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

void Sql::commit()
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

void Sql::rollback()
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
#endif
/* returns false on error */
void Sql::reportSqlError()
{
	if(db.lastError().isValid())
		throw DbError(db.lastError().text());
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


