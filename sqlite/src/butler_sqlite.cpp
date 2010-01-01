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

	bool Sqlite::createTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE Tags ("
				"name VARCHAR(32) PRIMARY KEY "
				")"
				);
		ret = reportSqlError();
/*
		query.exec("INSERT INTO Tags VALUES('élelmiszer')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('takarítószer')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('konyhafelszerelés')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('szeszesital')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('édesség')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('gép')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('barkács')");
		reportSqlError();
		query.exec("INSERT INTO Tags VALUES('ruházat')");
		reportSqlError();
*/
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("Tags");
		if(		!table.contains("name")
				) {
			ret = false;
			qCritical("Incompatible table Tags "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::createQueriesTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE Queries ("
				"query_name VARCHAR(64) NOT NULL, "
				"start_date DATE NOT NULL, "
				"end_date DATE NOT NULL"
				")"
				);
		ret = reportSqlError();
		/* the default query is planned to be tha last one, saved on exit */
/*		query.exec("INSERT INTO Queries VALUES('default', '2000-01-01', '2009-12-31')");
		reportSqlError();
*/
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkQueriesTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("Queries");
		if(		!table.contains("query_name") ||
				!table.contains("start_date") ||
				!table.contains("end_date")
				) {
			ret = false;
			qCritical("Incompatible table Queries "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::createQueryTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE QueryTags ("
				"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		ret = reportSqlError();
/*
		query.exec("INSERT INTO ItemTags VALUES('default', 'élelmiszer')");
		reportSqlError();
		query.exec("INSERT INTO ItemTags VALUES('default', 'szeszesital')");
		reportSqlError();
*/
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkQueryTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("QueryTags");
		if(		!table.contains("query_name") ||
				!table.contains("tag")
				) {
			ret = false;
			qCritical("Incompatible table QueryTags "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::createItemsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE Items ("
				"item VARCHAR(256) NOT NULL, "
				"uploaded DATE NOT NULL DEFAULT 'now', "
				"expected_price INT NOT NULL DEFAULT 0, "
				"purchased DATE NOT NULL DEFAULT 0, "
				"paid_price INT NOT NULL DEFAULT 0, "
				"amort_years INT NOT NULL DEFAULT 0, "
				"amort_months INT NOT NULL DEFAULT 0, "
				"amort_days INT NOT NULL DEFAULT 0, "
				"comments TEXT NOT NULL DEFAULT '')"
				);
		ret = reportSqlError();
/*		if(!query.exec("INSERT INTO Items"
				" (item, uploaded, expected_price, purchased, paid_price, amort_years, amort_months, amort_days, comments)"
				" VALUES('kenyér', date('now'), 200, date('now'), 190, 0, 0, 0, '')")){
			qCritical("SQL statement failed!");
			reportSqlError();
		}

		reportSqlError();

		query.exec("INSERT INTO Items"
				" (item, uploaded, expected_price, purchased, paid_price, amort_years, amort_months, amort_days, comments)"
				" VALUES('tej', date('now'), 200, date('now'), 190, 0, 0, 0, '')");
		query.exec("INSERT INTO Items"
				" (item, uploaded, expected_price, purchased, paid_price, amort_years, amort_months, amort_days, comments)"
				" VALUES('bor', date('now'), 200, date('now'), 190, 0, 0, 0, '')");
		query.exec("INSERT INTO Items"
				" (item, uploaded, expected_price, purchased, paid_price, amort_years, amort_months, amort_days, comments)"
				" VALUES('pálinka', date('now'), 200, date('now'), 190, 0, 0, 0, '')");
		*/
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkItemsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("Items");
		if(		!table.contains("item") ||
				!table.contains("uploaded") ||
				!table.contains("amort_years") ||
				!table.contains("amort_months") ||
				!table.contains("amort_days") ||
				!table.contains("comments")
				) {
			ret = false;
			qCritical("Incompatible table Items "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::createItemTagsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE ItemTags ("
				"item_id INT NOT NULL REFERENCES Items(ROWID), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		ret = reportSqlError();
/*
		query.exec("INSERT INTO ItemTags VALUES(1, 'élelmiszer')");
		reportSqlError();
		query.exec("INSERT INTO ItemTags VALUES(2, 'szeszesital')");
		reportSqlError();
*/
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkItemTagsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("ItemTags");
		if(		!table.contains("item_id") ||
				!table.contains("tag")
				) {
			ret = false;
			qCritical("Incompatible table ItemTags "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::createPurchasedItemsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE PurchasedItems ("
				"item_id INT NOT NULL REFERENCES Items(ROWID), "
				"purchased DATE NOT NULL DEFAULT 0, "
				"paid_price INT NOT NULL DEFAULT 0) "
				);
		ret = reportSqlError();
/*
		if(!query.exec("INSERT INTO Items"
				" (item_id, uploaded, expected_price, purchased, paid_price, amort_years, amort_months, amort_days, comments)"
				" VALUES(2, date('now'), 200)")){
			qCritical("SQL statement failed!");
			reportSqlError();
		}

		if(!query.exec("INSERT INTO Items"
				" (item_id, uploaded, expected_price, purchased, paid_price, amort_years, amort_months, amort_days, comments)"
				" VALUES(1, date('now'), 200)")){
			qCritical("SQL statement failed!");
			reportSqlError();
		}
*/
		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::checkPurchasedItemsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.record("PurchasedItems");
		if(		!table.contains("item_id") ||
				!table.contains("purchased") ||
				!table.contains("paid_price")
				) {
			ret = false;
			qCritical("Incompatible table PurchasedItems "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}
}


