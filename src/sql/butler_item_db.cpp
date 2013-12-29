/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item_db.h"

#include <math.h>
#include <float.h>

ItemDb::ItemDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("items"))
		sql.exec("CREATE TABLE items ("
				"uploaded TIMESTAMP NOT NULL PRIMARY KEY "
						"CHECK('1970-01-01T00:00:00' < uploaded), "
				"name TEXT NOT NULL, "
				"type TEXT NOT NULL, "
				"quantity DECIMAL(15,3) NOT NULL DEFAULT 0 CHECK(0 <= quantity), "
				"comment TEXT NOT NULL DEFAULT ''"
				")"
				);
	cols = sql.columns("items");
	if(		!cols.has("name") ||
			!cols.has("type") ||
			!cols.has("uploaded") ||
			!cols.has("quantity") ||
			!cols.has("comment")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table items in the openend database.");

	if(!tables.has("items_bought"))
		sql.exec("CREATE TABLE items_bought ("
				  "uploaded TIMESTAMP NOT NULL REFERENCES Items(uploaded) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "purchased TIMESTAMP NOT NULL, "
				  "price DECIMAL(15,2) NOT NULL DEFAULT 0 CHECK(0 <= price), "
				  "partner TEXT NOT NULL REFERENCES partners(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "on_stock INT NOT NULL DEFAULT 0 CHECK(on_stock = 0 OR on_stock = 1)"
				  ")"
			    );
	cols = sql.columns("items_bought");
	if(		!cols.has("uploaded") ||
			!cols.has("purchased") ||
			!cols.has("price") ||
			!cols.has("partner") ||
			!cols.has("on_stock")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table items_bought in the openend database.");
}

ItemDb::~ItemDb()
{
}

void ItemDb::insert(const Item & i)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO items (name, type, uploaded, "
			"quantity, comment) VALUES(?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, i.name);
	sqlQuery.bindValue(1, i.type);
	sqlQuery.bindValue(2, i.uploaded);
	sqlQuery.bindValue(3, i.quantity);
	sqlQuery.bindValue(4, i.comment);
	sqlQuery.exec();

	if(i.bought){
		sqlQuery.prepare("INSERT INTO items_bought "
				"(uploaded, purchased, price, partner, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");
		sqlQuery.bindValue(0, i.uploaded);
		sqlQuery.bindValue(1, i.purchased);
		sqlQuery.bindValue(2, i.price);
		sqlQuery.bindValue(3, i.partner);
		sqlQuery.bindValue(4, i.onStock ? 1 : 0);
		sqlQuery.exec();
	}

	tr.commit();
}

void ItemDb::update(const Item & orig, const Item & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* The orig and modified object should describe
	 * the same item's old and new content. */
	if(orig.uploaded != modified.uploaded)
		throw DbLogicError("The modified item is a different item than the original.");
	sqlQuery.prepare("UPDATE items SET "
			"name = ?, "
			"type = ?, "
			"quantity = ?, "
			"comment = ? "
			"WHERE uploaded = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.type);
	sqlQuery.bindValue(2, modified.quantity);
	sqlQuery.bindValue(3, modified.comment);
	sqlQuery.bindValue(4, orig.uploaded);
	sqlQuery.exec();

	if(!orig.bought && modified.bought){
		sqlQuery.prepare("INSERT INTO items_bought "
				"(uploaded, purchased, "
				"price, partner, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");
		sqlQuery.bindValue(0, modified.uploaded);
		sqlQuery.bindValue(1, modified.purchased);
		sqlQuery.bindValue(2, modified.price);
		sqlQuery.bindValue(3, modified.partner);
		sqlQuery.bindValue(4, modified.onStock ? 1 : 0);
		sqlQuery.exec();
	} else if(orig.bought && !modified.bought){
		sqlQuery.prepare("DELETE FROM items_bought WHERE uploaded = ?");
		sqlQuery.bindValue(0, orig.uploaded);
		sqlQuery.exec();
	} else if(orig.bought && modified.bought){
		/* The orig and modified object should describe
		 * the same item's old and new content. */
		if(orig.uploaded != modified.uploaded)
			throw DbLogicError(
					"The modified item is a different item than the original.");

		sqlQuery.prepare("UPDATE items_bought SET "
				"purchased = ?, "
				"price = ?, "
				"partner = ?, "
				"on_stock = ? "
				"WHERE uploaded = ?");
		sqlQuery.bindValue(0, modified.purchased);
		sqlQuery.bindValue(1, modified.price);
		sqlQuery.bindValue(2, modified.partner);
		sqlQuery.bindValue(3, modified.onStock ? 1 : 0);
		sqlQuery.bindValue(4, orig.uploaded);
		sqlQuery.exec();
	}

	tr.commit();
}

void ItemDb::del(const Item & i)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM items WHERE uploaded = ?");
	sqlQuery.bindValue(0, i.uploaded);
	sqlQuery.exec();

	tr.commit();
}

void ItemDb::query(const TagNameSet & tags, ItemSet & items)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* assemble command */
	QString cmd("SELECT "
			" MAX(items.uploaded) AS uploaded,"
			" MAX(items.name) AS name,"
			" MAX(items.type) AS type,"
			" MAX(items.comment) AS comment,"
			" MAX(items.quantity) AS quantity"
			" FROM items"
			" LEFT JOIN items_bought ON items.uploaded = items_bought.uploaded"
			" LEFT JOIN ware_tags ON items.name = ware_tags.name"
			" WHERE items_bought.uploaded IS NULL");
	{
		unsigned i, s = tags.size();
		if(s)
			cmd += " AND (";
		for(i=0; i<s; i++){
			if(0 < i)
				cmd += " OR";
			cmd += " tag = '";
			cmd += tags.queryAt(i).replace("'", "''");
			cmd += "'";
		}
		if(0 < s)
			cmd += ")";
	}
	cmd += " GROUP BY items.uploaded ORDER BY items.uploaded DESC";
	sqlQuery.exec(cmd);
	items.clear();
	int uploadedNo = sqlQuery.colIndex("uploaded");
	int nameNo = sqlQuery.colIndex("name");
	int typeNo = sqlQuery.colIndex("type");
	int quantityNo = sqlQuery.colIndex("quantity");
	int commentNo = sqlQuery.colIndex("comment");
	DBG("----- Item query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Item *item = new Item();
		item->uploaded = sqlQuery.dateTime(uploadedNo);
		item->name = sqlQuery.text(nameNo);
		item->type = sqlQuery.text(typeNo);
		item->quantity = sqlQuery.real(quantityNo);
		item->comment = sqlQuery.text(commentNo);
		items.add(item);
	}
	DBG("-----");

	tr.commit();
}

void ItemDb::query(const Query & q, QueryStat & stat, ItemSet & items)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

//	csjp::Time stopper = csjp::Time::unixTime();

	/* assemble command */
	QString cmd("SELECT"
			" MAX(items.uploaded) AS uploaded,"
			" MAX(items_bought.purchased) AS purchased,"
			" MAX(items.name) AS name,"
			" MAX(items.type) AS type,"
			" MAX(items.comment) AS comment,"
			" MAX(items.quantity) AS quantity,"
			" MAX(items_bought.price) AS price,"
			" MAX(items_bought.partner) AS partner,"
			" MAX(items_bought.on_stock) AS on_stock"
			" FROM items_bought"
			" LEFT JOIN items ON items_bought.uploaded = items.uploaded"
			" LEFT JOIN ware_tags ON items.name = ware_tags.name"
			" LEFT JOIN partners ON items_bought.partner = partners.name");
	
	QString filter;

	if(q.stockOption == Query::StockOptions::Gains){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '1'";
	} else if(q.stockOption == Query::StockOptions::Looses){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '0'";
	}

	if(!filter.isEmpty())
		filter += " AND";
	filter += " '";
	filter += q.startDate.isoUtcString();
	filter += "' < purchased";

	if(!filter.isEmpty())
		filter += " AND";
	filter += " purchased < '";
	filter += q.endDate.isoUtcString();
	filter += "'";

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
		unsigned i, s = q.partners.size();
		QString scmd;
		if(!filter.isEmpty())
			scmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				scmd += " OR";
			scmd += " Partners.name = '";
			scmd += q.partners.queryAt(i).replace("'", "''");
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
		tcmd += " Items.name NOT IN(SELECT name FROM ware_tags WHERE ";
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
	
	cmd += " GROUP BY items.uploaded";

	if(q.tagOption == Query::TagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd += QString::number(q.withTags.size());
	}

	sqlQuery.exec(cmd);

	items.clear();

	/* evaluate query result */
	int uploadedNo = sqlQuery.colIndex("uploaded");
	int purchasedNo = sqlQuery.colIndex("purchased");
	int nameNo = sqlQuery.colIndex("name");
	int typeNo = sqlQuery.colIndex("type");
	int commentNo = sqlQuery.colIndex("comment");
	int quantityNo = sqlQuery.colIndex("quantity");
	int priceNo = sqlQuery.colIndex("price");
	int partnerNo = sqlQuery.colIndex("partner");
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
	while (sqlQuery.next()) {
		DBG("Next row");
		Item *item = new Item();
		item->uploaded = sqlQuery.dateTime(uploadedNo);
		item->purchased = sqlQuery.dateTime(purchasedNo);
		item->name = sqlQuery.text(nameNo);
		item->type = sqlQuery.text(typeNo);
		item->comment = sqlQuery.text(commentNo);
		item->quantity = sqlQuery.real(quantityNo);
		item->price = sqlQuery.real(priceNo);
		item->partner = sqlQuery.text(partnerNo);
		item->onStock = sqlQuery.number(onStockNo);
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
	if(!stat.itemCount){
		stat.cheapestUnitPrice = NAN;
		stat.mostExpUnitPrice = NAN;
	}

	DBG("-----");
//	stat.queryTime = stopper - csjp::Time::unixTime();

	tr.commit();
}

/*
select * from (select Items.uploaded as id, Items.name as ware, Items.type as type, items_bought.purchased as bought, items_bought.price as price from Items left join items_bought on Items.uploaded = items_bought.uploaded left join WareTags on Items.name = WareTags.name where '2012-02-01T00:00:00' < items_bought.purchased and items_bought.purchased < '2012-03-01T00:00:00' and WareTags.tag = 'élelmiszer' group by Items.uploaded);
*/
