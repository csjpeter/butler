/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "butler_sqlite_item.h"

namespace Butler {
namespace Sqlite {
	
	ItemDb::ItemDb(
			Db &_db,
			TagDb &_tagDb) :
		db(_db),
		itemTagsDb(_db, _tagDb),
		purchasedItemDb(_db)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	ItemDb::~ItemDb()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	bool ItemDb::initializeTables(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret;

		if(!tables.contains("Items"))
			ret = createItemsTable();
		else
			ret = checkItemsTable();

		ret = ret && itemTagsDb.initializeTables(tables);
		ret = ret && purchasedItemDb.initializeTables(tables);

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemDb::createItemsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db.db);
		query.exec("CREATE TABLE Items ("
				"name VARCHAR(256) NOT NULL, "
				"uploaded DATE NOT NULL DEFAULT 'now', "
				"expected_price INT NOT NULL DEFAULT 0, "
				"amort_years INT NOT NULL DEFAULT 0, "
				"amort_months INT NOT NULL DEFAULT 0, "
				"amort_days INT NOT NULL DEFAULT 0, "
				"comment TEXT NOT NULL DEFAULT '')"
				);
		ret = db.reportSqlError();
		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemDb::checkItemsTable()
	{
		ENTER_FUNCTION();
		bool ret = true;

		QSqlRecord table = db.db.record("Items");
		if(		!table.contains("name") ||
				!table.contains("uploaded") ||
				!table.contains("expected_price") ||
				!table.contains("amort_years") ||
				!table.contains("amort_months") ||
				!table.contains("amort_days") ||
				!table.contains("comment")
				) {
			ret = false;
			qCritical("Incompatible table Items "
					"in the openend database.");
		}
		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemDb::insertItem(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "INSERT INTO Items ("
				"name, uploaded, expected_price, "
				"amort_years, amort_months, amort_days, "
				"comment"
				") "
				"VALUES('";
				query += i.name;
				query += "', '";
				query += i.uploaded.toString();
				query += "', '";
				query += i.expectedPrice;
				query += "', '";
				query += i.amortYears;
				query += "', '";
				query += i.amortMonths;
				query += "', '";
				query += i.amortDays;
				query += "', '";
				query += i.comment;
				query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		if(ret)
			ret = itemTagsDb.insertItemTags(i);
		if(ret && i.uploaded <= i.purchased) /* means: purchased/paid */
			ret = purchasedItemDb.insertPurchasedItem(i);

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemDb::updateItem(const Item &orig, const Item &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;
		int diffs = 0;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "UPDATE QueryOptions SET ";
		if(orig.name != modified.name){
			diffs++;
			query += "name = '";
			query += modified.name;
			query += "'";
		}
		if(orig.uploaded != modified.uploaded){
			if(diffs)
				query += ", ";
			diffs++;
			query += " uploaded = '";
			query += modified.uploaded.toString();
			query += "'";
		}
		if(orig.expectedPrice != modified.expectedPrice){
			if(diffs)
				query += ", ";
			diffs++;
			query += "expected_price = '";
			query += modified.expectedPrice;
			query += "'";
		}
		if(orig.amortYears != modified.amortYears){
			if(diffs)
				query += ", ";
			diffs++;
			query += "amort_years = '";
			query += modified.amortYears;
			query += "'";
		}
		if(orig.amortMonths != modified.amortMonths){
			if(diffs)
				query += ", ";
			diffs++;
			query += "amort_months = '";
			query += modified.amortMonths;
			query += "'";
		}
		if(orig.amortDays != modified.amortDays){
			if(diffs)
				query += ", ";
			diffs++;
			query += "amort_days = '";
			query += modified.amortDays;
			query += "'";
		}
		if(orig.comment != modified.comment){
			if(diffs)
				query += ", ";
			diffs++;
			query += "comment = '";
			query += modified.comment;
			query += "'";
		}
		query += " WHERE name = '";
		query += orig.name;
		query += "') ";
		if(diffs){
			sqlQuery.exec(query);
			ret = db.reportSqlError();
		}

		if(ret) {
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool ItemDb::deleteItem(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "DELETE FROM Items WHERE uploaded = '";
		query += i.uploaded.toString();
		query += "')";
		sqlQuery.exec(query);
		ret = db.reportSqlError();
	
		/* should be done automatically by a good SQL server */
		if(ret) {
		}

		LEAVE_FUNCTION();
		return ret;
	}

	ItemSet* ItemDb::queryItems(const Query &q)
	{
		ENTER_FUNCTION();

		/* assemble query */
		QString query("SELECT * FROM Items");

		if(!q.tags.empty()){
			query.append(" WHERE");
			int i, s = q.tags.size();
			query += " tag = '";
			query += q.tags.query(0).name;
			query += "'";
			for(i=1; i<s; i++){
				query += " OR";
				query += " tag = '";
				query += q.tags.query(i).name;
				query += "'";
			}
		}

		/* execute query */
		QSqlQuery sqlQuery(query, db.db);
		sqlQuery.exec();

		/* evaluate query result */
		int itemNo = sqlQuery.record().indexOf("name");
		int uploadedNo = sqlQuery.record().indexOf("uploaded");
		int expectedPriceNo = sqlQuery.record().indexOf("expected_price");
		int purchasedNo = sqlQuery.record().indexOf("purchased");
		int paidPriceNo = sqlQuery.record().indexOf("paid_price");
		int amortYearsNo = sqlQuery.record().indexOf("amort_years");
		int amortMonthsNo = sqlQuery.record().indexOf("amort_months");
		int amortDaysNo = sqlQuery.record().indexOf("amort_days");
		int commentNo = sqlQuery.record().indexOf("comment");

		ItemSet *items = new ItemSet;

		v1Debug("----- Item query result:");
		while (sqlQuery.next()) {
			Item *item = new Item();
			item->name = sqlQuery.value(itemNo).toString();
			item->uploaded = sqlQuery.value(uploadedNo).toDate();
			item->expectedPrice = sqlQuery.value(expectedPriceNo).toInt();
			item->purchased = sqlQuery.value(purchasedNo).toDate();
			item->paidPrice = sqlQuery.value(paidPriceNo).toInt();
			item->amortYears = sqlQuery.value(amortYearsNo).toUInt();
			item->amortMonths = sqlQuery.value(amortMonthsNo).toUInt();
			item->amortDays = sqlQuery.value(amortDaysNo).toUInt();
			item->comment = sqlQuery.value(commentNo).toString();
			items->append(item);
			v1Debug("Item: %s %s ", qPrintable(item->uploaded.toString()), qPrintable(item->name));
		}
		v1Debug("-----");

		LEAVE_FUNCTION();
		return items;
	}
}
}

