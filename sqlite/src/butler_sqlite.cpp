/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QDir>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "butler_sqlite.h"

#define CONNECTION_NAME "butler_sqlite_connection"

namespace Butler {

	Sqlite::Sqlite()
	{
	}

	Sqlite::Sqlite(const QString& path)
	{
		db = new QSqlDatabase;
		*db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
		reportSqlError();
		db->setDatabaseName(path);
		reportSqlError();
		db->open();
		reportSqlError();

		this->path = path;
		initializeTables();
	}

	Sqlite::~Sqlite()
	{
		db->close();
		reportSqlError();
		delete db;
		db = 0;
		QSqlDatabase::removeDatabase(CONNECTION_NAME);
	}

	/*
	 *	Private members
	 */

	void Sqlite::reportSqlError()
	{
		if(db->lastError().isValid()){
			qCritical("%s", qPrintable(db->lastError().text()));
		}
	}

	void Sqlite::initializeTables()
	{
		QStringList tables = db->tables();

		if(!tables.contains("Items"))
			createItemsTable();
		else
			checkItemsTable();

		if(!tables.contains("PurchasedItems"))
			createPurchasedItemsTable();
		else
			checkPurchasedItemsTable();

		if(!tables.contains("Tags"))
			createTagsTable();
		else
			checkTagsTable();

		if(!tables.contains("ItemTags"))
			createItemTagsTable();
		else
			checkItemTagsTable();

		if(!tables.contains("Queries"))
			createQueriesTable();
		else
			checkQueriesTable();

		if(!tables.contains("QueryTags"))
			createQueryTagsTable();
		else
			checkQueryTagsTable();
	}

	void Sqlite::createItemsTable()
	{
		QSqlQuery query(*db);
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
		reportSqlError();
		if(!query.exec("INSERT INTO Items"
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
	}

	void Sqlite::checkItemsTable()
	{
		QSqlRecord table = db->record("Items");
		if(		!table.contains("item") ||
				!table.contains("uploaded") ||
				!table.contains("amort_years") ||
				!table.contains("amort_months") ||
				!table.contains("amort_days") ||
				!table.contains("comments")
				) {
			qCritical("Incompatible table Items in the openend database.");
		}
	}

	void Sqlite::createPurchasedItemsTable()
	{
		QSqlQuery query(*db);
		query.exec("CREATE TABLE PurchasedItems ("
				"item_id INT NOT NULL REFERENCES Items(ROWID), "
				"purchased DATE NOT NULL DEFAULT 0, "
				"paid_price INT NOT NULL DEFAULT 0) "
				);
		reportSqlError();
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
	}

	void Sqlite::checkPurchasedItemsTable()
	{
		QSqlRecord table = db->record("PurchasedItems");
		if(		!table.contains("item_id") ||
				!table.contains("purchased") ||
				!table.contains("paid_price")
				) {
			qCritical("Incompatible table PurchasedItems in the openend database.");
		}
	}

	void Sqlite::createTagsTable()
	{
		QSqlQuery query(*db);
		query.exec("CREATE TABLE Tags ("
				"name VARCHAR(32) PRIMARY KEY) "
				")"
				);
		reportSqlError();
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
	}

	void Sqlite::checkTagsTable()
	{
		QSqlRecord table = db->record("Tags");
		if(		!table.contains("name")
				) {
			qCritical("Incompatible table Tags in the openend database.");
		}
	}

	void Sqlite::createItemTagsTable()
	{
		QSqlQuery query(*db);
		query.exec("CREATE TABLE ItemTags ("
				"item_id INT NOT NULL REFERENCES Items(ROWID), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		reportSqlError();
		query.exec("INSERT INTO ItemTags VALUES(1, 'élelmiszer')");
		reportSqlError();
		query.exec("INSERT INTO ItemTags VALUES(2, 'szeszesital')");
		reportSqlError();
	}

	void Sqlite::checkItemTagsTable()
	{
		QSqlRecord table = db->record("ItemTags");
		if(		!table.contains("item_id") ||
				!table.contains("tag")
				) {
			qCritical("Incompatible table ItemTags in the openend database.");
		}
	}

	void Sqlite::createQueriesTable()
	{
		QSqlQuery query(*db);
		query.exec("CREATE TABLE Queries ("
				"query_name VARCHAR(64) NOT NULL, "
				"start_date DATE NOT NULL, "
				"end_date DATE NOT NULL"
				")"
				);
		reportSqlError();
		/* the default query is planned to be tha last one, saved on exit */
		query.exec("INSERT INTO Queries VALUES('default', '2000-01-01', '2009-12-31')");
		reportSqlError();
	}

	void Sqlite::checkQueriesTable()
	{
		QSqlRecord table = db->record("Queries");
		if(		!table.contains("query_name") ||
				!table.contains("start_date") ||
				!table.contains("end_date")
				) {
			qCritical("Incompatible table Queries in the openend database.");
		}
	}

	void Sqlite::createQueryTagsTable()
	{
		QSqlQuery query(*db);
		query.exec("CREATE TABLE QueryTags ("
				"query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name), "
				"tag VARCHAR(32) REFERENCES Tags(name)"
				")"
				);
		reportSqlError();
		query.exec("INSERT INTO ItemTags VALUES('default', 'élelmiszer')");
		reportSqlError();
		query.exec("INSERT INTO ItemTags VALUES('default', 'szeszesital')");
		reportSqlError();
	}

	void Sqlite::checkQueryTagsTable()
	{
		QSqlRecord table = db->record("QueryTags");
		if(		!table.contains("query_name") ||
				!table.contains("tag")
				) {
			qCritical("Incompatible table QueryTags in the openend database.");
		}
	}
}


