/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <ButlerDebug>
#include "butler_sqlite_db.h"

#define CONNECTION_NAME "butler_sqlite_connection"

namespace Butler {
namespace Sqlite {

#ifdef DEBUG
	void listAvailableFeatures(const QSqlDatabase &db);
#endif

	Db::Db(const QString& _path)
	{
		ENTER_CONSTRUCTOR();
		path = _path;
		LEAVE_CONSTRUCTOR();
	}

	Db::~Db()
	{
		ENTER_DESTRUCTOR();
		if(db.isOpen())
			close();
		LEAVE_DESTRUCTOR();
	}
	
	bool Db::connect()
	{
		ENTER_FUNCTION();
		Q_ASSERT(!db.isValid());
		bool ret;

		if(!QSqlDatabase::contains(CONNECTION_NAME)){
			db = QSqlDatabase::addDatabase(
					"QSQLITE", CONNECTION_NAME);
			ret = reportSqlError();
		} else {
			db = QSqlDatabase::database(CONNECTION_NAME, false);
			ret = reportSqlError();
		}
		Q_ASSERT(ret == db.isValid());
		
		if(ret){
			db.setDatabaseName(path);
			ret = reportSqlError();
		}

#ifdef DEBUG
		if(ret)
			listAvailableFeatures(db);
#endif

		LEAVE_FUNCTION();
		return ret;
	}

	bool Db::open()
	{
		ENTER_FUNCTION();
		Q_ASSERT(db.isValid());
		bool ret;

		db.open();
		ret = reportSqlError();
		Q_ASSERT(ret == db.isOpen());

		LEAVE_FUNCTION();
		return ret;
	}

	bool Db::close()
	{
		ENTER_FUNCTION();
		Q_ASSERT(db.isOpen());
		bool ret;

		db.close();
		ret = reportSqlError();
		Q_ASSERT(ret = !db.isOpen());
		
		LEAVE_FUNCTION();
		return ret;
	}
			
	const QString& Db::lastError()
	{
		ENTER_FUNCTION();
		LEAVE_FUNCTION();
		return lastErr;
	}

	/*
	 *	Private members
	 */
	
/*	Db::Db()
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}
*/
	bool Db::reportSqlError()
	{
		ENTER_FUNCTION();
		bool ret = true;
		if(db.lastError().isValid()){
			lastErr = db.lastError().text();
			qCritical("%s", qPrintable(lastErr));
			ret = false;
		}
		LEAVE_FUNCTION();
		return ret;
	}

#ifdef DEBUG
	void listAvailableFeatures(const QSqlDatabase &db)
	{
		static bool featuresListed = false;

		if(featuresListed)
			return;

		QSqlDriver *drv = db.driver();
		Q_ASSERT(drv != NULL);
		QString format = "QSqlDriver features for connection %s :";
		int i;
		for(i=0; i<14; i++)
			format += "\n\t%-25s%3d";

		qDebug(qPrintable(format),
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
}


