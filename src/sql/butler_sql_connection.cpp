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

#define DEBUG

#include "butler_sql_connection.h"

#ifdef DEBUG
void listAvailableFeatures(const QSqlDatabase &db, const DatabaseDescriptor & dbDesc);
#endif

SqlConnection::SqlConnection(const DatabaseDescriptor & dbDesc) :
	dbDesc(dbDesc),
	transactions(0)
{
	ENSURE(!db.isValid(), csjp::LogicError);

#ifdef DEBUG
	DBG("Available drivers: %s", C_STR((QSqlDatabase::drivers().join(", "))));
#endif

	if(!QSqlDatabase::contains(dbDesc.name)){
		db = QSqlDatabase::addDatabase(dbDesc.driver, dbDesc.name);
		if(db.lastError().isValid())
			throw DbError("Failed to add SQLITE database driver: %s",
					C_STR(dbErrorString()));
	} else {
		db = QSqlDatabase::database(dbDesc.name, false);
		if(db.lastError().isValid())
			throw DbError("Failed to get database connection named '%s'.\nError: %s",
					C_STR(dbDesc.name), C_STR(dbErrorString()));
	}
	ENSURE(db.isValid(), csjp::LogicError);

	open();
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
}

void SqlConnection::open()
{
	if(db.isOpen()) return;

	ENSURE(db.isValid(), csjp::LogicError);

	db.setDatabaseName(dbDesc.databaseName);
	if(db.lastError().isValid())
		throw DbError("Failed to set database name to %s.\nError: %s",
				C_STR(dbDesc.databaseName),
				C_STR(dbErrorString()));

	if(dbDesc.driver != "QSQLITE"){
		db.setHostName(dbDesc.host);
		db.setUserName(dbDesc.username);
		db.setPassword(dbDesc.password);
		db.setPort(dbDesc.port);
		db.setConnectOptions("sslmode=require");
	} else {
		db.setPort(-1);
	}

#ifdef DEBUG
	listAvailableFeatures(db, dbDesc);
#endif

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

		if(q.value(0).toInt() != 1){
			db.close();
			throw DbError("Reference constraits could not turned "
					"on or not supported at all.");
		}
	}
}

void SqlConnection::close()
{
	if(!db.isOpen()) return;

	ENSURE(db.isOpen(), csjp::LogicError);

	db.close();
	if(db.lastError().isValid())
		throw DbError("Failed to close database '%s'.\nError: %s",
				C_STR(db.databaseName()),
				C_STR(dbErrorString()));

	ENSURE(!db.isOpen(), csjp::LogicError);
}

/*
 *	Private members
 */

QSqlQuery* SqlConnection::createQuery()
{
	open();

	QSqlQuery * q = new QSqlQuery(db);
	if(!q)
		throw DbError("Failed to create QSqlQuery object");
	return q;
}

void SqlConnection::exec(const QString &query)
{
	open();

	csjp::Object<QSqlQuery> qQuery(new QSqlQuery(db));
	qQuery->setForwardOnly(true);

	DBG("%s", C_STR(query));
	if(!qQuery->exec(query))
		throw DbError("The below sql query failed:\n%s\nDatabase reports error: %s",
				C_STR(query),
				C_STR(dbErrorString()));
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
	open();

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

#ifdef DEBUG
void listAvailableFeatures(const QSqlDatabase &db, const DatabaseDescriptor & dbDesc)
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
