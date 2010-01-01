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

#include "butler_sqlite.h"

namespace Butler {

	static void _append_tag_match_test(QString& query, const QString& tag)
	{
		query.append(" tag = '");
		query.append(tag);
		query.append("'");
	}

	ItemSet* Sqlite::queryItems(const QueryOptions &qo)
	{
		/* assemble query */
		QString query("SELECT * FROM Items");

		if(!qo.tags.empty()){
			query.append(" WHERE");
			int i, s = qo.tags.size();
			_append_tag_match_test(query, qo.tags.query(0).name);
			for(i=1; i<s; i++){
				query.append(" OR");
				_append_tag_match_test(query, qo.tags.query(i).name);
			}
		}

		/* execute query */
		QSqlQuery sqlQuery(query, db);
		sqlQuery.exec();

		/* evaluate query result */
		int itemNo = sqlQuery.record().indexOf("item");
		int uploadedNo = sqlQuery.record().indexOf("uploaded");
		int expectedPriceNo = sqlQuery.record().indexOf("expected_price");
		int purchasedNo = sqlQuery.record().indexOf("purchased");
		int paidPriceNo = sqlQuery.record().indexOf("paid_price");
		int amortYearsNo = sqlQuery.record().indexOf("amort_years");
		int amortMonthsNo = sqlQuery.record().indexOf("amort_months");
		int amortDaysNo = sqlQuery.record().indexOf("amort_days");
		int commentsNo = sqlQuery.record().indexOf("comments");

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
			item->comment = sqlQuery.value(commentsNo).toString();
			items->append(item);
			v1Debug("Item: %s %s ", qPrintable(item->uploaded.toString()), qPrintable(item->name));
		}
		v1Debug("-----");

		return items;
	}
}


