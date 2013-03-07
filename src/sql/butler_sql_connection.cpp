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

#include "butler_sql_connection.h"

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

SqlConnection::SqlConnection(const DatabaseDescription & dbDesc) :
	dbDesc(dbDesc),
	transactions(0)
{
	ENSURE(!db.isValid(), csjp::LogicError);

	if(!QSqlDatabase::contains(dbDesc.name)){
		db = QSqlDatabase::addDatabase(dbDesc.driver, dbDesc.name);
		if(db.lastError().isValid())
			throw DbError("Failed to add SQLITE database driver: %s",
					C_STR(dbErrorString()));
	} else {
		db = QSqlDatabase::database(dbDesc.name, false);
		if(db.lastError().isValid())
			throw DbError("Failed to get database connection named '%s'.\nError: %s",
					dbDesc.name, C_STR(dbErrorString()));
	}
	ENSURE(db.isValid(), csjp::LogicError);
	
	db.setDatabaseName(dbDesc.databaseName);
	if(db.lastError().isValid())
		throw DbError("Failed to set database name to %s.\nError: %s",
				C_STR(dbDesc.databaseName),
				C_STR(dbErrorString()));

#ifdef DEBUG
	listAvailableFeatures(db);
#endif
}

SqlConnection::~SqlConnection()
{
	if(db.isOpen()){
		LOG("Closing db connection at destruction time. "
				"This can easily cause a crash.");
		close();
	}
}

void SqlConnection::open()
{
	if(db.isOpen()) return;

//	ENSURE(db.isValid(), csjp::LogicError);

	db.open();
	if(db.lastError().isValid())
		throw DbError("Failed to open database '%s'.\nError: %s",
				C_STR(db.databaseName()),
				C_STR(dbErrorString()));

	ENSURE(db.isOpen(), csjp::LogicError);

	if(dbDesc.driver == "QSQLITE"){
		/* Lets check if the sqlite on system is compatible with us. */
		/* Lets turn on reference constraits */
		QSqlQuery q = db.exec("PRAGMA foreign_keys = ON");
		q = db.exec("PRAGMA foreign_keys");
		ENSURE(q.isActive(), csjp::LogicError);
		ENSURE(q.isSelect(), csjp::LogicError);

		q.next();

		DBG("Reference constraint support: %s",
				(q.value(0).toInt()) ? "yes" : "no");
	}

	if(q.value(0).toInt() != 1){
		db.close();
		throw DbError("Reference constraits could not turned "
			"on or not supported at all.");
	}
}

void SqlConnection::close()
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
	QSqlDatabase::removeDatabase(dbDesc.name);
}

/*
 *	Private members
 */

/*SqlConnection::SqlConnection()
{
}
*/

QSqlQuery* SqlConnection::createQuery()
{
	QSqlQuery * q = new QSqlQuery(db);
	if(!q)
		throw DbError("Failed to create QSqlQuery object");
	return q;
}

QSqlRecord SqlConnection::record(const QString &tablename) const
{
	return db.record(tablename);
}

QStringList SqlConnection::tables() const
{
	return db.tables();
}

bool SqlConnection::isOpen()
{
	return db.isOpen();
}

void SqlConnection::transaction()
{
	if(transactions == 0){
		DBG("BEGIN TRANSACTION");
		db.transaction();
		if(db.lastError().isValid())
			throw DbError("Failed begin transcation.\nError: %s",
					C_STR(dbErrorString()));
	}

	transactions++;
}

void SqlConnection::commit()
{
	if(transactions == 1){
		DBG("COMMIT TRANSACTION");
		db.commit();
		if(db.lastError().isValid())
			throw DbError("Failed to commit transcation.\nError: %s",
					C_STR(dbErrorString()));
	}

	transactions--;
}

void SqlConnection::rollback()
{
	if(transactions == 1){
		DBG("ROLLBACK TRANSACTION");
#if 0
		notifySqlFinishListeners();
#endif
		db.rollback();
		if(db.lastError().isValid())
			LOG("Failed to rollback transcation.\nError: %s", C_STR(dbErrorString()));
			/* FIXME test this */
/*			throw DbError("Failed to rollback transcation.\nError: %s",
					C_STR(dbErrorString()));
*/
	}

	transactions--;
}

QString SqlConnection::dbErrorString()
{
	return db.lastError().text();
}

void SqlConnection::addSqlCloseListener(SqlCloseListener &l)
{
	sqlCloseListeners.add(l);
}

void SqlConnection::removeSqlCloseListener(SqlCloseListener &l)
{
	sqlCloseListeners.remove(l);
}
#if 0	
void SqlConnection::notifySqlFinishListeners()
{
	unsigned s,i;
	s = sqlFinishListeners.size();
	for(i=0; i<s; i++)
		sqlFinishListeners.queryAt(i).sqlFinishNotification();
}
#endif			
void SqlConnection::notifySqlCloseListeners()
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
