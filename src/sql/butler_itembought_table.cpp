/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

//#include <csjp_time.h>

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "butler_itembought_table.h"

ItemBoughtTable::ItemBoughtTable(SqlConnection &_sql) :
	sql(_sql)
{
}

ItemBoughtTable::~ItemBoughtTable()
{
}

void ItemBoughtTable::check(QStringList &tables)
{
	if(!tables.contains("ItemsBought"))
		sql.exec("CREATE TABLE ItemsBought ("
				  "uploaded DATE NOT NULL REFERENCES Items(uploaded) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "purchased DATE NOT NULL, "
				  "price REAL NOT NULL DEFAULT 0 CHECK(0 <= price), "
				  "shop VARCHAR(64) NOT NULL REFERENCES Shops(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "on_stock INT NOT NULL DEFAULT 0 CHECK(on_stock = 0 OR on_stock = 1)"
				  ")"
			    );

	QSqlRecord table = sql.record("ItemsBought");
	if(		!table.contains("uploaded") ||
			!table.contains("purchased") ||
			!table.contains("price") ||
			!table.contains("shop") ||
			!table.contains("on_stock")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table ItemsBought in the openend database.");
}

void ItemBoughtTable::insert(const Item &i)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO ItemsBought "
				"(uploaded, purchased, "
				"price, shop, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, i.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	insertQuery.bindValue(1, i.purchased.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	insertQuery.bindValue(2, i.price);
	insertQuery.bindValue(3, i.shop);
	insertQuery.bindValue(4, i.onStock ? 1 : 0);
	insertQuery.exec();
}

void ItemBoughtTable::update(const Item &orig, const Item &modified)
{
	SqlQuery updateQuery(sql);
	/* The orig and modified object should describe
	 * the same item's old and new content. */
	if(orig.uploaded.toString() != modified.uploaded.toString())
		throw DbLogicError("The modified item is a different item than the original.");

	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE ItemsBought SET "
				"purchased = ?, "
				"price = ?, "
				"shop = ?, "
				"on_stock = ? "
				"WHERE uploaded = ?");

	updateQuery.bindValue(0, modified.purchased.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	updateQuery.bindValue(1, modified.price);
	updateQuery.bindValue(2, modified.shop);
	updateQuery.bindValue(3, modified.onStock ? 1 : 0);
	updateQuery.bindValue(4, orig.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	updateQuery.exec();
}

void ItemBoughtTable::del(const Item &i)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare("DELETE FROM ItemsBought WHERE uploaded = ?");

	deleteQuery.bindValue(0, i.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	deleteQuery.exec();
}

void ItemBoughtTable::query(const Query &q, QueryStat &stat, ItemSet &items)
{
//	csjp::Time stopper = csjp::Time::unixTime();
	SqlQuery sqlQuery(sql);

	/* assemble command */
	QString cmd("SELECT * FROM ItemsBought"
			" LEFT JOIN Items ON ItemsBought.uploaded = Items.uploaded"
			" LEFT JOIN WareTags ON Items.name = WareTags.name"
			" LEFT JOIN Shops ON ItemsBought.shop = Shops.name");
	
	QString filter;

	if(q.stockOption == Query::ITEMS_ON_STOCK){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '1'";
	} else if(q.stockOption == Query::ITEMS_USED_UP){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '0'";
	}

	if(q.startDate.isValid()){
		if(!filter.isEmpty())
			filter += " AND";
		filter +=  " '" + QDateTime(q.startDate).toUTC().toString("yyyy-MM-ddThh:mm:ss") + "' < purchased";
	}
	if(q.endDate.isValid()){
		if(!filter.isEmpty())
			filter += " AND";
		filter +=  " purchased < '" + QDateTime(q.endDate).toUTC().toString("yyyy-MM-ddThh:mm:ss") + "'";
	}

	{
		unsigned i, s = q.withTags.size();
		QString tcmd;
		if(!filter.isEmpty())
			tcmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				tcmd += " OR";
			tcmd += " tag = '";
			tcmd += q.withTags.queryAt(i).replace("'", "''");
			tcmd += "'";
		}
		tcmd.append(")");
		if(0 < s)
			filter.append(tcmd);
	}

	{
		unsigned i, s = q.wares.size();
		QString wcmd;
		if(!filter.isEmpty())
			wcmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				wcmd += " OR";
			wcmd += " Items.name = '";
			wcmd += q.wares.queryAt(i).replace("'", "''");
			wcmd += "'";
		}
		wcmd.append(")");
		if(0 < s)
			filter.append(wcmd);
	}

	{
		unsigned i, s = q.shops.size();
		QString scmd;
		if(!filter.isEmpty())
			scmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				scmd += " OR";
			scmd += " Shops.name = '";
			scmd += q.shops.queryAt(i).replace("'", "''");
			scmd += "'";
		}
		scmd.append(")");
		if(0 < s)
			filter.append(scmd);
	}

	{
		unsigned i, s = q.withoutTags.size();
		QString tcmd;
		if(!filter.isEmpty())
			tcmd += " AND";
		tcmd += " Items.name NOT IN(SELECT name FROM WareTags WHERE ";
		for(i=0; i<s; i++){
			if(0 < i)
				tcmd += " OR";
			tcmd += " tag = '";
			tcmd += q.withoutTags.queryAt(i).replace("'", "''");
			tcmd += "'";
		}
		tcmd += " GROUP BY name)";
		if(0 < s)
			filter.append(tcmd);
	}

	if(!filter.isEmpty()){
		cmd += " WHERE";
		cmd += filter;
	}
	
	cmd += " GROUP BY Items.uploaded";

	if(q.tagOption == Query::MATCH_ALL_TAGS){
		cmd += " HAVING COUNT(*) = ";
		cmd += QString::number(q.withTags.size());
	}

	sqlQuery.exec(cmd);

	items.clear();

	/* evaluate query result */
	int uploadedNo = sqlQuery.colIndex("uploaded");
	int purchasedNo = sqlQuery.colIndex("purchased");
	int nameNo = sqlQuery.colIndex("name");
	int categoryNo = sqlQuery.colIndex("category");
	int commentNo = sqlQuery.colIndex("comment");
	int quantityNo = sqlQuery.colIndex("quantity");
	int priceNo = sqlQuery.colIndex("price");
	int shopNo = sqlQuery.colIndex("shop");
	int onStockNo = sqlQuery.colIndex("on_stock");

	/* statistics */
	stat.itemCount = 0;
	stat.sumQuantity = 0;
	stat.sumPrice = 0;
	stat.cheapestUnitPrice = DBL_MAX;
	stat.mostExpUnitPrice = 0;
	double sumPrice = 0;
	double sumQuantity = 0;

	DBG("----- Item query result:");
	QDateTime dt;
	while (sqlQuery.next()) {
		DBG("Next row");
		Item *item = new Item();

		dt = sqlQuery.value(uploadedNo).toDateTime();
		dt.setTimeSpec(Qt::UTC);
		item->uploaded = dt.toLocalTime();

		dt = sqlQuery.value(purchasedNo).toDateTime();
		dt.setTimeSpec(Qt::UTC);
		item->purchased = dt.toLocalTime();

		item->name = sqlQuery.value(nameNo).toString();
		item->category = sqlQuery.value(categoryNo).toString();
		item->comment = sqlQuery.value(commentNo).toString();

		item->quantity = sqlQuery.value(quantityNo).toDouble();
		item->price = sqlQuery.value(priceNo).toDouble();
		item->shop = sqlQuery.value(shopNo).toString();
		item->onStock = sqlQuery.value(onStockNo).toBool();

		item->bought = true;

		/* statistics */
		stat.itemCount++;
		stat.sumQuantity += item->quantity;
		stat.sumPrice += item->price;
		if(DBL_EPSILON <= item->quantity && DBL_EPSILON <= item->price){
			sumQuantity += item->quantity;
			sumPrice += item->price;

			double unitPrice = item->price / item->quantity;
			if(unitPrice < stat.cheapestUnitPrice)
				stat.cheapestUnitPrice = unitPrice;
			if(stat.mostExpUnitPrice < unitPrice)
				stat.mostExpUnitPrice = unitPrice;
		}

		items.add(item);
	}

	stat.avgPrice = sumPrice / sumQuantity;

	DBG("-----");
//	stat.queryTime = stopper - csjp::Time::unixTime();
}

/*
select * from (select Items.uploaded as id, Items.name as ware, Items.category as category, ItemsBought.purchased as bought, ItemsBought.price as price from Items left join ItemsBought on Items.uploaded = ItemsBought.uploaded left join WareTags on Items.name = WareTags.name where '2012-02-01T00:00:00' < ItemsBought.purchased and ItemsBought.purchased < '2012-03-01T00:00:00' and WareTags.tag = 'élelmiszer' group by Items.uploaded);
*/
