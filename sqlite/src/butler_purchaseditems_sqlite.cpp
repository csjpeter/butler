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

#include "ButlerSqlite"

namespace Butler {
	
	bool Sqlite::createPurchasedItemsTable()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db);
		query.exec("CREATE TABLE PurchasedItems ("
				"uploaded DATE NOT NULL REFERENCES Items(uploaded), "
				"purchased DATE NOT NULL DEFAULT 0, "
				"paid_price INT NOT NULL DEFAULT 0) "
				);
		ret = reportSqlError();

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

	bool Sqlite::insertItemPurchased(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret = true;

		Q_UNUSED(i);

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::updateItemPurchased(const Item &orig, const Item &modified)
	{
		ENTER_FUNCTION();
		bool ret = true;

		Q_UNUSED(orig);
		Q_UNUSED(modified);

		LEAVE_FUNCTION();
		return ret;
	}

	bool Sqlite::deleteItemPurchased(const Item &i)
	{
		ENTER_FUNCTION();
		bool ret = true;

		Q_UNUSED(i);

		LEAVE_FUNCTION();
		return ret;
	}

}

