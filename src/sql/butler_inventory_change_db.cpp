/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include "butler_inventory_change_db.h"

#include <math.h>
#include <float.h>

InventoryChangeDb::InventoryChangeDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("inventoryChanges"))
		sql.exec("CREATE TABLE inventoryChanges ("
				"upload_date TIMESTAMP NOT NULL CHECK('1970-01-01T00:00:00' < upload_date)"
				"name TEXT NOT NULL, "
				"unit TEXT NOT NULL, "
				"type TEXT NOT NULL, "
				"brand TEXT NOT NULL, "
				"quantity DECIMAL(15,3) NOT NULL, "
				"price DECIMAL(15,2) NOT NULL, "
				"currency TEXT NOT NULL, "
				"account TEXT NOT NULL, "
				"partner TEXT NOT NULL, "
				"inventory TEXT NOT NULL, "
				"comment TEXT NOT NULL DEFAULT '',"
				"inv_change_date TIMESTAMP, "
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
				"" // keys
				"PRIMARY KEY (upload_date), "
				"FOREIGN KEY (name, unit) REFERENCES wares(name, unit) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"FOREIGN KEY (name, type) REFERENCES ware_types(name, type) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"FOREIGN KEY (account, currency) REFERENCES accounts(name, currency) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"FOREIGN KEY (partner) REFERENCES partners(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"FOREIGN KEY (inventory) REFERENCES inventories(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				")"
				);
	cols = sql.columns("inventoryChanges");
	if(		!cols.has("upload_date") ||
			!cols.has("name") ||
			!cols.has("unit") ||
			!cols.has("type") ||
			!cols.has("brand") ||
			!cols.has("quantity") ||
			!cols.has("price") ||
			!cols.has("currency") ||
			!cols.has("account") ||
			!cols.has("partner") ||
			!cols.has("inventory") ||
			!cols.has("comment") ||
			!cols.has("inv_change_date") ||
			!cols.has("last_modified")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table inventoryChanges in the openend database.");
}

InventoryChangeDb::~InventoryChangeDb()
{
}

void InventoryChangeDb::insert(const InventoryChange & i)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO inventoryChanges (upload_date, name, unit, type, brand, "
			"quantity, price, currency, account, partner, inventory, comment, inv_change_date) "
			"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, i.uploadDate);
	sqlQuery.bindValue(1, i.unit);
	sqlQuery.bindValue(2, i.name);
	sqlQuery.bindValue(3, i.type);
	sqlQuery.bindValue(4, i.brand);
	sqlQuery.bindValue(5, i.quantity);
	sqlQuery.bindValue(6, i.price);
	sqlQuery.bindValue(7, i.currency);
	sqlQuery.bindValue(8, i.account);
	sqlQuery.bindValue(9, i.partner);
	sqlQuery.bindValue(10, i.inventory);
	sqlQuery.bindValue(11, i.comment);
	sqlQuery.bindValue(12, i.invChangeDate);
	sqlQuery.exec();

	tr.commit();
}

void InventoryChangeDb::update(const InventoryChange & orig, const InventoryChange & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* The orig and modified object should describe
	 * the same inventoryChange's old and new content. */
	if(orig.uploadDate != modified.uploadDate)
		throw DbLogicError("The modified inventoryChange is a different "
						"inventoryChange than the original.");
	sqlQuery.prepare("UPDATE inventoryChanges SET "
					"name = ?, "
					"unit = ?, "
					"type = ?, "
					"brand = ?, "
					"quantity = ?, "
					"price = ?, "
					"currency = ?, "
					"account = ?, "
					"partner = ?, "
					"inventory = ?, "
					"comment = ?, "
					"inv_change_date = ?, "
					"WHERE uploadDate = ?");
	sqlQuery.bindValue(0, orig.name);
	sqlQuery.bindValue(1, orig.unit);
	sqlQuery.bindValue(2, orig.type);
	sqlQuery.bindValue(3, orig.brand);
	sqlQuery.bindValue(4, orig.quantity);
	sqlQuery.bindValue(5, orig.price);
	sqlQuery.bindValue(6, orig.currency);
	sqlQuery.bindValue(7, orig.account);
	sqlQuery.bindValue(8, orig.partner);
	sqlQuery.bindValue(9, orig.inventory);
	sqlQuery.bindValue(10, orig.comment);
	sqlQuery.bindValue(11, orig.invChangeDate);
	sqlQuery.bindValue(12, orig.uploadDate);
	sqlQuery.exec();

	tr.commit();
}

void InventoryChangeDb::del(const InventoryChange & i)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM inventoryChanges WHERE uploadDate = ?");
	sqlQuery.bindValue(0, i.uploadDate);
	sqlQuery.exec();

	tr.commit();
}

void InventoryChangeDb::query(const TagNameSet & tags, InventoryChangeSet & inventoryChanges)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* assemble command */
	QString cmd("SELECT "
			" MAX(inventoryChanges.upload_date) AS upload_date,"
			" MAX(inventoryChanges.name) AS name,"
			" MAX(inventoryChanges.unit) AS unit,"
			" MAX(inventoryChanges.type) AS type,"
			" MAX(inventoryChanges.brand) AS brand,"
			" MAX(inventoryChanges.quantity) AS quantity, "
			" MAX(inventoryChanges.price) AS price,"
			" MAX(inventoryChanges.currency) AS currency,"
			" MAX(inventoryChanges.account) AS account,"
			" MAX(inventoryChanges.partner) AS partner,"
			" MAX(inventoryChanges.inventory) AS inventory,"
			" MAX(inventoryChanges.comment) AS comment,"
			" MAX(inventoryChanges.inv_change_date) AS inv_change_date"
			" FROM inventoryChanges"
			" LEFT JOIN ware_tags ON inventoryChanges.name = ware_tags.name"
			" WHERE ");
	{
		unsigned i, s = tags.size();
		if(s)
			cmd += "  (";
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
	cmd += " GROUP BY inventoryChanges.upload_date ORDER BY inventoryChanges.upload_date DESC";
	sqlQuery.exec(cmd);
	inventoryChanges.clear();
	int uploadDateNo = sqlQuery.colIndex("upload_date");
	int nameNo = sqlQuery.colIndex("name");
	int unitNo = sqlQuery.colIndex("unit");
	int typeNo = sqlQuery.colIndex("type");
	int brandNo = sqlQuery.colIndex("brand");
	int quantityNo = sqlQuery.colIndex("quantity");
	int priceNo = sqlQuery.colIndex("price");
	int currencyNo = sqlQuery.colIndex("currency");
	int accountNo = sqlQuery.colIndex("account");
	int partnerNo = sqlQuery.colIndex("partner");
	int inventoryNo = sqlQuery.colIndex("inventory");
	int commentNo = sqlQuery.colIndex("comment");
	int invChangeDateNo = sqlQuery.colIndex("inv_change_date");
	DBG("----- InventoryChange query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		InventoryChange *inventoryChange = new InventoryChange();
		inventoryChange->uploadDate = sqlQuery.dateTime(uploadDateNo);
		inventoryChange->name = sqlQuery.text(nameNo);
		inventoryChange->unit = sqlQuery.text(unitNo);
		inventoryChange->type = sqlQuery.text(typeNo);
		inventoryChange->brand = sqlQuery.text(brandNo);
		inventoryChange->quantity = sqlQuery.real(quantityNo);
		inventoryChange->price = sqlQuery.text(priceNo);
		inventoryChange->currency = sqlQuery.text(currencyNo);
		inventoryChange->account = sqlQuery.text(accountNo);
		inventoryChange->partner = sqlQuery.text(partnerNo);
		inventoryChange->inventory = sqlQuery.text(inventoryNo);
		inventoryChange->comment = sqlQuery.text(commentNo);
		inventoryChange->invChangeDate = sqlQuery.dateTime(invChangeDateNo);
		inventoryChanges.add(inventoryChange);
	}
	DBG("-----");

	tr.commit();
}

void InventoryChangeDb::query(const Query & q, QueryStat & stat, InventoryChangeSet & inventoryChanges)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

//	csjp::Time stopper = csjp::Time::unixTime();

	/* assemble command */
	QString cmd("SELECT"
			" MAX(inventoryChanges.upload_date) AS upload_date,"
			" MAX(inventoryChanges.name) AS name,"
			" MAX(inventoryChanges.unit) AS unit,"
			" MAX(inventoryChanges.type) AS type,"
			" MAX(inventoryChanges.brand) AS brand,"
			" MAX(inventoryChanges.quantity) AS quantity, "
			" MAX(inventoryChanges.price) AS price,"
			" MAX(inventoryChanges.currency) AS currency,"
			" MAX(inventoryChanges.account) AS account,"
			" MAX(inventoryChanges.partner) AS partner,"
			" MAX(inventoryChanges.inventory) AS inventory,"
			" MAX(inventoryChanges.comment) AS comment,"
			" MAX(inventoryChanges.inv_change_date) AS inv_change_date"
			" FROM inventoryChanges"
			" LEFT JOIN ware_tags ON inventoryChanges.name = ware_tags.name"
			" LEFT JOIN partners ON inventoryChanges.partner = partners.name");
	
	QString filter;

	if(q.stockOption == Query::StockOptions::Gains){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " 0 < quantity";
	} else if(q.stockOption == Query::StockOptions::Looses){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " quantity < 0";
	}

	if(!filter.isEmpty())
		filter += " AND";
	filter += " '";
	filter += q.startDate.isoUtcString();
	filter += "' < inv_change_date";

	if(!filter.isEmpty())
		filter += " AND";
	filter += " inv_change_date < '";
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
			wcmd += " InventoryChanges.name = '";
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
		tcmd += " InventoryChanges.name NOT IN(SELECT name FROM ware_tags WHERE ";
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
	
	cmd += " GROUP BY inventoryChanges.upload_date";

	if(q.tagOption == Query::TagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd += QString::number(q.withTags.size());
	}

	sqlQuery.exec(cmd);

	inventoryChanges.clear();

	/* evaluate query result */
	int uploadDateNo = sqlQuery.colIndex("upload_date");
	int nameNo = sqlQuery.colIndex("name");
	int unitNo = sqlQuery.colIndex("unit");
	int typeNo = sqlQuery.colIndex("type");
	int brandNo = sqlQuery.colIndex("brand");
	int quantityNo = sqlQuery.colIndex("quantity");
	int priceNo = sqlQuery.colIndex("price");
	int currencyNo = sqlQuery.colIndex("currency");
	int accountNo = sqlQuery.colIndex("account");
	int partnerNo = sqlQuery.colIndex("partner");
	int inventoryNo = sqlQuery.colIndex("inventory");
	int commentNo = sqlQuery.colIndex("comment");
	int invChangeDateNo = sqlQuery.colIndex("inv_change_date");

	/* statistics */
	stat.inventoryChangeCount = 0;
	stat.sumQuantity = 0;
	stat.sumPrice = 0;
	stat.cheapestUnitPrice = DBL_MAX;
	stat.mostExpUnitPrice = 0;
	double sumPrice = 0;
	double sumQuantity = 0;

	DBG("----- InventoryChange query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		InventoryChange *inventoryChange = new InventoryChange();
		inventoryChange->uploadDate = sqlQuery.dateTime(uploadDateNo);
		inventoryChange->name = sqlQuery.text(nameNo);
		inventoryChange->unit = sqlQuery.text(unitNo);
		inventoryChange->type = sqlQuery.text(typeNo);
		inventoryChange->brand = sqlQuery.text(brandNo);
		inventoryChange->quantity = sqlQuery.real(quantityNo);
		inventoryChange->price = sqlQuery.text(priceNo);
		inventoryChange->currency = sqlQuery.text(currencyNo);
		inventoryChange->account = sqlQuery.text(accountNo);
		inventoryChange->partner = sqlQuery.text(partnerNo);
		inventoryChange->inventory = sqlQuery.text(inventoryNo);
		inventoryChange->comment = sqlQuery.text(commentNo);
		inventoryChange->invChangeDate = sqlQuery.dateTime(invChangeDateNo);

		/* statistics */
		stat.inventoryChangeCount++;
		stat.sumQuantity += inventoryChange->quantity;
		stat.sumPrice += inventoryChange->price;
		if(DBL_EPSILON <= inventoryChange->quantity && DBL_EPSILON <= inventoryChange->price){
			sumQuantity += inventoryChange->quantity;
			sumPrice += inventoryChange->price;

			double unitPrice = inventoryChange->price / inventoryChange->quantity;
			if(unitPrice < stat.cheapestUnitPrice)
				stat.cheapestUnitPrice = unitPrice;
			if(stat.mostExpUnitPrice < unitPrice)
				stat.mostExpUnitPrice = unitPrice;
		}

		inventoryChanges.add(inventoryChange);
	}

	stat.avgPrice = sumPrice / sumQuantity;
	if(!stat.inventoryChangeCount){
		stat.cheapestUnitPrice = NAN;
		stat.mostExpUnitPrice = NAN;
	}

	DBG("-----");
//	stat.queryTime = stopper - csjp::Time::unixTime();

	tr.commit();
}

/*
select * from (select InventoryChanges.upload_date as id, InventoryChanges.name as ware, InventoryChanges.type as type, inventoryChanges_bought.purchased as bought, inventoryChanges_bought.price as price from InventoryChanges left join inventoryChanges_bought on InventoryChanges.upload_date = inventoryChanges_bought.upload_date left join WareTags on InventoryChanges.name = WareTags.name where '2012-02-01T00:00:00' < inventoryChanges_bought.purchased and inventoryChanges_bought.purchased < '2012-03-01T00:00:00' and WareTags.tag = 'élelmiszer' group by InventoryChanges.upload_date);
*/
