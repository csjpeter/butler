/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include "butler_inventoryChange_db.h"

#include <math.h>
#include <float.h>

InventoryChangeDb::InventoryChangeDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("inventoryChanges"))
		sql.exec("CREATE TABLE inventoryChanges ("
				"uploadDate TIMESTAMP NOT NULL PRIMARY KEY "
						"CHECK('1970-01-01T00:00:00' < uploadDate), "
				"name TEXT NOT NULL, "
				"type TEXT NOT NULL, "
				"brand TEXT NOT NULL, "
				"quantity DECIMAL(15,3) NOT NULL DEFAULT 0 CHECK(0 <= quantity), "
				"partner TEXT NOT NULL REFERENCES partners(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"inventory TEXT NOT NULL REFERENCES inventories(name) "
				"ON DELETE RESTRICT ON UPDATE CASCADE, "
				"comment TEXT NOT NULL DEFAULT '',"
				"invChangeDate TIMESTAMP NOT NULL, "
				"lastModified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP "
				")"
				);
	cols = sql.columns("inventoryChanges");
	if(		( !cols.has("uploadDate") && !cols.has("uploaddate") ) ||
			!cols.has("name") ||
			!cols.has("type") ||
			!cols.has("brand") ||
			!cols.has("quantity") ||
			!cols.has("partner") ||
			!cols.has("inventory") ||
			!cols.has("comment") ||
			( !cols.has("invChangeDate") && !cols.has("invchangedate") ) ||
			( !cols.has("lastModified") && !cols.has("lastmodified") )
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

	sqlQuery.prepare("INSERT INTO inventoryChanges (uploadDate, name, type, brand, "
			"partner, inventory, quantity, comment, invChangeDate) "
			"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, i.uploadDate);
	sqlQuery.bindValue(1, i.name);
	sqlQuery.bindValue(2, i.type);
	sqlQuery.bindValue(3, i.brand);
	sqlQuery.bindValue(4, i.quantity);
	sqlQuery.bindValue(5, i.partner);
	sqlQuery.bindValue(6, i.inventory);
	sqlQuery.bindValue(7, i.comment);
	sqlQuery.bindValue(8, i.invChangeDate);
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
					"type = ?, "
					"brand = ?, "
					"quantity = ?, "
					"partner = ?, "
					"inventory = ?, "
					"comment = ?, "
					"invChangeDate = ?, "
					"WHERE uploadDate = ?");
	sqlQuery.bindValue(0, orig.name);
	sqlQuery.bindValue(1, orig.type);
	sqlQuery.bindValue(2, orig.brand);
	sqlQuery.bindValue(3, orig.quantity);
	sqlQuery.bindValue(4, orig.partner);
	sqlQuery.bindValue(5, orig.inventory);
	sqlQuery.bindValue(6, orig.comment);
	sqlQuery.bindValue(7, orig.invChangeDate);
	sqlQuery.bindValue(8, orig.uploadDate);
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
			" MAX(inventoryChanges.uploadDate) AS uploadDate,"
			" MAX(inventoryChanges.name) AS name,"
			" MAX(inventoryChanges.type) AS type,"
			" MAX(inventoryChanges.brand) AS brand,"
			" MAX(inventoryChanges.quantity) AS quantity, "
			" MAX(inventoryChanges.partner) AS partner,"
			" MAX(inventoryChanges.inventory) AS inventory,"
			" MAX(inventoryChanges.comment) AS comment,"
			" MAX(inventoryChanges.invChangeDate) AS invChangeDate"
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
	cmd += " GROUP BY inventoryChanges.uploadDate ORDER BY inventoryChanges.uploadDate DESC";
	sqlQuery.exec(cmd);
	inventoryChanges.clear();
	int uploadDateNo = sqlQuery.colIndex("uploadDate");
	int nameNo = sqlQuery.colIndex("name");
	int typeNo = sqlQuery.colIndex("type");
	int brandNo = sqlQuery.colIndex("brand");
	int quantityNo = sqlQuery.colIndex("quantity");
	int partnerNo = sqlQuery.colIndex("partner");
	int inventoryNo = sqlQuery.colIndex("inventory");
	int commentNo = sqlQuery.colIndex("comment");
	int invChangeDateNo = sqlQuery.colIndex("invChangeDate");
	DBG("----- InventoryChange query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		InventoryChange *inventoryChange = new InventoryChange();
		inventoryChange->uploadDate = sqlQuery.dateTime(uploadDateNo);
		inventoryChange->name = sqlQuery.text(nameNo);
		inventoryChange->type = sqlQuery.text(typeNo);
		inventoryChange->brand = sqlQuery.text(brandNo);
		inventoryChange->quantity = sqlQuery.real(quantityNo);
		inventoryChange->partner = sqlQuery.text(partnerNo);
		inventoryChange->inventory = sqlQuery.text(inventoryNo);
		inventoryChange->comment = sqlQuery.text(commentNo);
		inventoryChange->invChangeDate = sqlQuery.text(invChangeDateNo);
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
			" MAX(inventoryChanges.uploadDate) AS uploadDate,"
			" MAX(inventoryChanges.name) AS name,"
			" MAX(inventoryChanges.type) AS type,"
			" MAX(inventoryChanges.brand) AS brand,"
			" MAX(inventoryChanges.quantity) AS quantity, "
			" MAX(inventoryChanges.partner) AS partner,"
			" MAX(inventoryChanges.inventory) AS inventory,"
			" MAX(inventoryChanges.comment) AS comment,"
			" MAX(inventoryChanges.invChangeDate) AS invChangeDate"
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
	filter += "' < invChangeDate";

	if(!filter.isEmpty())
		filter += " AND";
	filter += " invChangeDate < '";
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
	
	cmd += " GROUP BY inventoryChanges.uploadDate";

	if(q.tagOption == Query::TagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd += QString::number(q.withTags.size());
	}

	sqlQuery.exec(cmd);

	inventoryChanges.clear();

	/* evaluate query result */
	int uploadDateNo = sqlQuery.colIndex("uploadDate");
	int nameNo = sqlQuery.colIndex("name");
	int typeNo = sqlQuery.colIndex("type");
	int brandNo = sqlQuery.colIndex("brand");
	int quantityNo = sqlQuery.colIndex("quantity");
	int partnerNo = sqlQuery.colIndex("partner");
	int inventoryNo = sqlQuery.colIndex("inventory");
	int commentNo = sqlQuery.colIndex("comment");
	int invChangeDateNo = sqlQuery.colIndex("invChangeDate");

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
		inventoryChange->type = sqlQuery.text(typeNo);
		inventoryChange->brand = sqlQuery.text(brandNo);
		inventoryChange->quantity = sqlQuery.real(quantityNo);
		inventoryChange->partner = sqlQuery.text(partnerNo);
		inventoryChange->inventory = sqlQuery.text(inventoryNo);
		inventoryChange->comment = sqlQuery.text(commentNo);
		inventoryChange->invChangeDate = sqlQuery.text(invChangeDateNo);

		/* statistics */
		stat.inventoryChangeCount++;
		stat.sumQuantity += inventoryChange->quantity;
		/*stat.sumPrice += inventoryChange->price;
		if(DBL_EPSILON <= inventoryChange->quantity && DBL_EPSILON <= inventoryChange->price){
			sumQuantity += inventoryChange->quantity;
			sumPrice += inventoryChange->price;

			double unitPrice = inventoryChange->price / inventoryChange->quantity;
			if(unitPrice < stat.cheapestUnitPrice)
				stat.cheapestUnitPrice = unitPrice;
			if(stat.mostExpUnitPrice < unitPrice)
				stat.mostExpUnitPrice = unitPrice;
		}*/

		inventoryChanges.add(inventoryChange);
	}
/*
	stat.avgPrice = sumPrice / sumQuantity;
	if(!stat.inventoryChangeCount){
		stat.cheapestUnitPrice = NAN;
		stat.mostExpUnitPrice = NAN;
	}
*/
	DBG("-----");
//	stat.queryTime = stopper - csjp::Time::unixTime();

	tr.commit();
}

/*
select * from (select InventoryChanges.uploadDate as id, InventoryChanges.name as ware, InventoryChanges.type as type, inventoryChanges_bought.purchased as bought, inventoryChanges_bought.price as price from InventoryChanges left join inventoryChanges_bought on InventoryChanges.uploadDate = inventoryChanges_bought.uploadDate left join WareTags on InventoryChanges.name = WareTags.name where '2012-02-01T00:00:00' < inventoryChanges_bought.purchased and inventoryChanges_bought.purchased < '2012-03-01T00:00:00' and WareTags.tag = 'élelmiszer' group by InventoryChanges.uploadDate);
*/
