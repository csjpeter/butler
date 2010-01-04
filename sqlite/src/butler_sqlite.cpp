/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <ButlerDebug>
#include <ButlerSqlite>

#define CONNECTION_NAME "butler_sqlite_connection"

namespace Butler {

	Sqlite::Sqlite(const QString& _path)
	{
		ENTER_CONSTRUCTOR();
		path = _path;
		LEAVE_CONSTRUCTOR();
	}

	Sqlite::~Sqlite()
	{
		ENTER_DESTRUCTOR();
		if(db.isOpen())
			close();
		if(db.isValid())
			QSqlDatabase::removeDatabase(CONNECTION_NAME);
		LEAVE_DESTRUCTOR();
	}
	
	bool Sqlite::connect()
	{
		ENTER_FUNCTION();
		Q_ASSERT(!db.isValid());
		bool ret;

		db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
		ret = reportSqlError();
		Q_ASSERT(ret == db.isValid());

		if(ret){
			db.setDatabaseName(path);
			ret = reportSqlError();
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::open()
	{
		ENTER_FUNCTION();
		Q_ASSERT(db.isValid());
		bool ret;

		db.open();
		ret = reportSqlError();
		Q_ASSERT(ret == db.isOpen());
		
		if(ret && !initializeTables()){
			close();
			ret = false;
			lastError = "Could not initialize tables for butler.";
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::close()
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

	/*
	 *	Private members
	 */
	
/*	Sqlite::Sqlite()
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}
*/
	bool Sqlite::reportSqlError()
	{
		ENTER_FUNCTION();
		bool ret = true;
		if(db.lastError().isValid()){
			lastError = db.lastError().text();
			qCritical("%s", qPrintable(lastError));
			ret = false;
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::initializeTables()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QStringList tables = db.tables();

		if(!tables.contains("Tags"))
			ret = createTagsTable() && ret;
		else
			ret = checkTagsTable() && ret;

		if(!tables.contains("Queries"))
			ret = createQueriesTable() && ret;
		else
			ret = checkQueriesTable() && ret;

		if(!tables.contains("QueryTags"))
			ret = createQueryTagsTable() && ret;
		else
			ret = checkQueryTagsTable() && ret;

		if(!tables.contains("Items"))
			ret = createItemsTable() && ret;
		else
			ret = checkItemsTable() && ret;

		if(!tables.contains("ItemTags"))
			ret = createItemTagsTable() && ret;
		else
			ret = checkItemTagsTable() && ret;

		if(!tables.contains("PurchasedItems"))
			ret = createPurchasedItemsTable() && ret;
		else
			ret = checkPurchasedItemsTable() && ret;

		LEAVE_FUNCTION();
		return ret;
	}
}


