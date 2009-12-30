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

	QList<Item*>* Sqlite::queryItemList(const QueryOptions &qo)
	{
		/* assemble query */
		QString query("SELECT rowid,* FROM Items");

		if(!qo.empty()){
			query.append(" WHERE");
			QueryOptions::const_iterator tag = qo.constBegin();
			_append_tag_match_test(query, *tag);
			for(QueryOptions::const_iterator tag = qo.constBegin();
					tag != qo.constEnd();
					tag++){
				query.append(" OR");
				_append_tag_match_test(query, *tag);
			}
		}

		/* execute query */
		QSqlQuery sqlQuery(query, *db);
		sqlQuery.exec();

		/* evaluate query result */
		int idNo = sqlQuery.record().indexOf("rowid");
		int itemNo = sqlQuery.record().indexOf("item");
		int uploadedNo = sqlQuery.record().indexOf("uploaded");
		int expectedPriceNo = sqlQuery.record().indexOf("expected_price");
		int purchasedNo = sqlQuery.record().indexOf("purchased");
		int paidPriceNo = sqlQuery.record().indexOf("paid_price");
		int amortYearsNo = sqlQuery.record().indexOf("amort_years");
		int amortMonthsNo = sqlQuery.record().indexOf("amort_months");
		int amortDaysNo = sqlQuery.record().indexOf("amort_days");
		int commentsNo = sqlQuery.record().indexOf("comments");

		QList<Item*> *items = new QList<Item*>;

		qDebug("----- Item query result:");

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
			item->comments = sqlQuery.value(commentsNo).toString();

			items->append(item);

			qDebug("Item: %s ", item->id, qPrintable(item->name));
		}

		qDebug("-----");

		return items;
	}
}


