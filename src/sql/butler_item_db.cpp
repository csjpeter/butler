/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item_db.h"

ItemDb::ItemDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("items"))
		sql.exec("CREATE TABLE items ("
				"uploaded TIMESTAMP NOT NULL PRIMARY KEY "
						"CHECK('1970-01-01T00:00:00' < uploaded), "
				"name VARCHAR(256) NOT NULL, "
				"category VARCHAR(256) NOT NULL, "
				"quantity REAL NOT NULL DEFAULT 0 CHECK(0 <= quantity), "
				"comment TEXT NOT NULL DEFAULT ''"
				")"
				);
	cols = sql.columns("items");
	if(		!cols.has("name") ||
			!cols.has("category") ||
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
				  "price REAL NOT NULL DEFAULT 0 CHECK(0 <= price), "
				  "partner VARCHAR(64) NOT NULL REFERENCES partners(name) "
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
	SqlTransaction tr(sql);

	SqlQuery insertQuery(sql);
	insertQuery.prepare("INSERT INTO items (name, category, uploaded, "
			"quantity, comment) VALUES(?, ?, ?, ?, ?)");
	insertQuery.bindValue(0, i.name);
	insertQuery.bindValue(1, i.category);
	insertQuery.bindValue(2, i.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
	insertQuery.bindValue(3, i.quantity);
	insertQuery.bindValue(4, i.comment);
	insertQuery.exec();

	if(i.bought){
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO items_bought "
				"(uploaded, purchased, "
				"price, partner, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");
		insertQuery.bindValue(0, i.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		insertQuery.bindValue(1, i.purchased.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		insertQuery.bindValue(2, i.price);
		insertQuery.bindValue(3, i.partner);
		insertQuery.bindValue(4, i.onStock ? 1 : 0);
		insertQuery.exec();
	}

	tr.commit();
}

void ItemDb::update(const Item & orig, const Item & modified)
{
	SqlTransaction tr(sql);

	SqlQuery updateQuery(sql);
	/* The orig and modified object should describe
	 * the same item's old and new content. */
	if(orig.uploaded.toString() != modified.uploaded.toString())
		throw DbLogicError("The modified item is a different item than the original.");
	updateQuery.prepare("UPDATE items SET "
			"name = ?, "
			"category = ?, "
			"quantity = ?, "
			"comment = ? "
			"WHERE uploaded = ?");
	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.category);
	updateQuery.bindValue(2, modified.quantity);
	updateQuery.bindValue(3, modified.comment);
	updateQuery.bindValue(4, orig.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
	updateQuery.exec();

	if(!orig.bought && modified.bought){
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO items_bought "
				"(uploaded, purchased, "
				"price, partner, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");
		insertQuery.bindValue(0, modified.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		insertQuery.bindValue(1, modified.purchased.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		insertQuery.bindValue(2, modified.price);
		insertQuery.bindValue(3, modified.partner);
		insertQuery.bindValue(4, modified.onStock ? 1 : 0);
		insertQuery.exec();
	} else if(orig.bought && !modified.bought){
		SqlQuery deleteQuery(sql);
		deleteQuery.prepare("DELETE FROM items_bought WHERE uploaded = ?");
		deleteQuery.bindValue(0, orig.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		deleteQuery.exec();
	} else if(orig.bought && modified.bought){
		SqlQuery updateQuery(sql);
		/* The orig and modified object should describe
		 * the same item's old and new content. */
		if(orig.uploaded.toString() != modified.uploaded.toString())
			throw DbLogicError(
					"The modified item is a different item than the original.");

		updateQuery.prepare("UPDATE items_bought SET "
				"purchased = ?, "
				"price = ?, "
				"partner = ?, "
				"on_stock = ? "
				"WHERE uploaded = ?");
		updateQuery.bindValue(0, modified.purchased.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		updateQuery.bindValue(1, modified.price);
		updateQuery.bindValue(2, modified.partner);
		updateQuery.bindValue(3, modified.onStock ? 1 : 0);
		updateQuery.bindValue(4, orig.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
		updateQuery.exec();
	}

	tr.commit();
}

void ItemDb::del(const Item & i)
{
	SqlTransaction tr(sql);

	SqlQuery deleteQuery(sql);
	deleteQuery.prepare("DELETE FROM items WHERE uploaded = ?");
	deleteQuery.bindValue(0, i.uploaded.toUTC().toString("yyyy-MM-dd hh:mm:ss"));
	deleteQuery.exec();

	tr.commit();
}

void ItemDb::query(const TagNameSet & tags, ItemSet & items)
{
	SqlTransaction tr(sql);

	SqlQuery sqlQuery(sql);
	/* assemble command */
	QString cmd("SELECT "
			" MAX(items.uploaded) AS uploaded,"
			" MAX(items.name) AS name,"
			" MAX(items.category) AS category,"
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
	int categoryNo = sqlQuery.colIndex("category");
	int quantityNo = sqlQuery.colIndex("quantity");
	int commentNo = sqlQuery.colIndex("comment");
	DBG("----- Item query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		QDateTime dt;
		Item *item = new Item();

		dt = sqlQuery.value(uploadedNo).toDateTime();
		dt.setTimeSpec(Qt::UTC);
		item->uploaded = dt.toLocalTime();

		item->name = sqlQuery.value(nameNo).toString();
		item->category = sqlQuery.value(categoryNo).toString();
		item->quantity = sqlQuery.value(quantityNo).toDouble();
		item->comment = sqlQuery.value(commentNo).toString();
		items.add(item);
	}
	DBG("-----");

	tr.commit();
}

void ItemDb::query(const Query & q, QueryStat & stat, ItemSet & items)
{
	SqlTransaction tr(sql);

//	csjp::Time stopper = csjp::Time::unixTime();
	SqlQuery sqlQuery(sql);

	/* assemble command */
	QString cmd("SELECT"
			" MAX(items.uploaded) AS uploaded,"
			" MAX(items_bought.purchased) AS purchased,"
			" MAX(items.name) AS name,"
			" MAX(items.category) AS category,"
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

	if(q.stockOption == Query::StockOptions::ItemsOnStock){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '1'";
	} else if(q.stockOption == Query::StockOptions::ItemsUsedUp){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '0'";
	}

	if(q.startDate.isValid()){
		if(!filter.isEmpty())
			filter += " AND";
		filter +=  " '" + QDateTime(q.startDate).toUTC().toString("yyyy-MM-dd hh:mm:ss") + "' < purchased";
	}
	if(q.endDate.isValid()){
		if(!filter.isEmpty())
			filter += " AND";
		filter +=  " purchased < '" + QDateTime(q.endDate).toUTC().toString("yyyy-MM-dd hh:mm:ss") + "'";
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
	int categoryNo = sqlQuery.colIndex("category");
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
		item->partner = sqlQuery.value(partnerNo).toString();
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
	if(!stat.itemCount){
		stat.cheapestUnitPrice = NAN;
		stat.mostExpUnitPrice = NAN;
	}

	DBG("-----");
//	stat.queryTime = stopper - csjp::Time::unixTime();

	tr.commit();
}

/*
select * from (select Items.uploaded as id, Items.name as ware, Items.category as category, items_bought.purchased as bought, items_bought.price as price from Items left join items_bought on Items.uploaded = items_bought.uploaded left join WareTags on Items.name = WareTags.name where '2012-02-01T00:00:00' < items_bought.purchased and items_bought.purchased < '2012-03-01T00:00:00' and WareTags.tag = 'élelmiszer' group by Items.uploaded);
*/
