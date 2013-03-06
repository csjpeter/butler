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

#include <butler_query.h>

#include "butler_sqlite_queries.h"

namespace Sqlite {

QueryTable::QueryTable(Sql &sql) :
	sql(sql),
	insertQuery(sql),
	updateQuery(sql),
	deleteQuery(sql),
	selectAllQuery(sql)
{
}

QueryTable::~QueryTable()
{
}

void QueryTable::check(QStringList &tables)
{
	if(!tables.contains("Queries"))
		sql.exec("CREATE TABLE Queries ("
				  "query_name VARCHAR(64) NOT NULL PRIMARY KEY, "
				  "stock_option INTEGER NOT NULL, "
				  "tag_option INTEGER NOT NULL, "
				  "start_date DATE NOT NULL, "
				  "end_date DATE NOT NULL"
				  ")"
			    );

	QSqlRecord table = sql.record("Queries");
	if(		!table.contains("query_name") ||
			!table.contains("stock_option") ||
			!table.contains("tag_option") ||
			!table.contains("start_date") ||
			!table.contains("end_date")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table Queries in the openend database.");
}

void QueryTable::insert(const Query &q)
{
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Queries "
			"(query_name, stock_option, tag_option, start_date, end_date) "
			"VALUES (?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, q.stockOption);
	insertQuery.bindValue(2, q.tagOption);
	insertQuery.bindValue(3, q.startDate.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	insertQuery.bindValue(4, q.endDate.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	insertQuery.exec();
	insertQuery.finish();
}

void QueryTable::update(const Query &orig, const Query &modified)
{
	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Queries SET "
				"query_name = ?, "
				"stock_option = ?, "
				"tag_option = ?, "
				"start_date = ?, "
				"end_date = ? "
				"WHERE query_name = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.stockOption);
	updateQuery.bindValue(2, modified.tagOption);
	updateQuery.bindValue(3, modified.startDate.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	updateQuery.bindValue(4, modified.endDate.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	updateQuery.bindValue(5, orig.name);
	updateQuery.exec();
	updateQuery.finish();
}

void QueryTable::del(const Query &q)
{
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM Queries WHERE "
				"query_name = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.exec();
	deleteQuery.finish();
}

void QueryTable::query(QuerySet &queries)
{
	if(!selectAllQuery.isPrepared())
		selectAllQuery.prepare("SELECT "
			"query_name, stock_option, tag_option, start_date, end_date "
			"FROM Queries");

	selectAllQuery.exec();

	queries.clear();

	int queryNameNo = selectAllQuery.colIndex("query_name");
	int stockOptionNo = selectAllQuery.colIndex("stock_option");
	int tagOptionNo = selectAllQuery.colIndex("tag_option");
	int startDateNo = selectAllQuery.colIndex("start_date");
	int endDateNo = selectAllQuery.colIndex("end_date");

	DBG("----- Query query result:");
	QDateTime dt;
	while (selectAllQuery.next()) {
		DBG("Next row");
		Query *query = new Query();
		query->name = selectAllQuery.value(queryNameNo).toString();
		query->stockOption = (enum Query::StockOptions)selectAllQuery.value(
				stockOptionNo).toInt();
		query->tagOption = (enum Query::TagOptions)selectAllQuery.value(
				tagOptionNo).toInt();

		dt = selectAllQuery.value(startDateNo).toDateTime();
		dt.setTimeSpec(Qt::UTC);
		query->startDate = dt.toLocalTime();

		dt = selectAllQuery.value(endDateNo).toDateTime();
		dt.setTimeSpec(Qt::UTC);
		query->endDate = dt.toLocalTime();

		queries.add(query);
	}
	DBG("-----");

	selectAllQuery.finish();
}

}

