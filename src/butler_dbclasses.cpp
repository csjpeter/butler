/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <math.h>
#include <float.h>

#include <butler_dbclasses.h>

@declare@ Tag
@include@ dbclass.cpp

@declare@ WareType
@include@ dbclass.cpp

@declare@ WareTag
@include@ dbclass.cpp

@declare@ Ware
@include@ dbclass.cpp

@declare@ Company
@include@ dbclass.cpp

@declare@ Brand
@include@ dbclass.cpp

@declare@ Inventory
@include@ dbclass.cpp

@declare@ Partner
@include@ dbclass.cpp

@declare@ Account
@include@ dbclass.cpp

@declare@ Payment
@include@ dbclass.cpp

@declare@ Item
@include@ dbclass.cpp

void ItemDb::query(const Query & q, QueryStat & stat, ItemSet & set)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

//	csjp::Time stopper = csjp::Time::unixTime();

	/* assemble command */
	QString cmd("SELECT"
			" MAX(Item.uploadDate) AS uploadDate,"
			" MAX(Item.name) AS name,"
			" MAX(Item.unit) AS unit,"
			" MAX(Item.type) AS type,"
			" MAX(Item.brand) AS brand,"
			" MAX(Item.quantity) AS quantity, "
			" MAX(Item.price) AS price,"
			" MAX(Item.currency) AS currency,"
			" MAX(Item.account) AS account,"
			" MAX(Item.partner) AS partner,"
			" MAX(Item.inventory) AS inventory,"
			" MAX(Item.comment) AS comment,"
			" MAX(Item.invChangeDate) AS invChangeDate"
			" FROM Item"
			" LEFT JOIN WareTag ON Item.name = WareTag.ware"
			" LEFT JOIN Partner ON Item.partner = Partner.name");
	
	QString filter;

	if(q.stockOption == QueryStockOptions::Gains){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " 0 < quantity";
	} else if(q.stockOption == QueryStockOptions::Looses){
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
			tcmd += q.withTags.queryAt(i).tag.replace("'", "''");
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
			wcmd += " Item.name = '";
			wcmd += q.wares.queryAt(i).ware.replace("'", "''");
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
			scmd += " partners.name = '";
			scmd += q.partners.queryAt(i).partner.replace("'", "''");
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
		tcmd += " Item.name NOT IN(SELECT name FROM WareTag WHERE ";
		for(i=0; i<s; i++){
			if(0 < i)
				tcmd += " OR";
			tcmd += " tag = '";
			tcmd += q.withoutTags.queryAt(i).tag.replace("'", "''");
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
	
	cmd += " GROUP BY Item.uploadDate";

	if(q.tagOption == QueryTagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd += QString::number(q.withTags.size());
	}

	sqlQuery.exec(cmd);

	set.clear();

	/* evaluate query result */
	int uploadDateNo = sqlQuery.colIndex("uploadDate");
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
	int invChangeDateNo = sqlQuery.colIndex("invChangeDate");

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
		Item *inventoryChange = new Item();
		inventoryChange->uploadDate = sqlQuery.dateTime(uploadDateNo);
		inventoryChange->name = sqlQuery.text(nameNo);
		inventoryChange->unit = sqlQuery.text(unitNo);
		inventoryChange->type = sqlQuery.text(typeNo);
		inventoryChange->brand = sqlQuery.text(brandNo);
		inventoryChange->quantity = sqlQuery.real(quantityNo);
		inventoryChange->price = sqlQuery.real(priceNo);
		inventoryChange->currency = sqlQuery.text(currencyNo);
		inventoryChange->account = sqlQuery.text(accountNo);
		inventoryChange->partner = sqlQuery.text(partnerNo);
		inventoryChange->inventory = sqlQuery.text(inventoryNo);
		inventoryChange->comment = sqlQuery.text(commentNo);
		inventoryChange->invChangeDate = sqlQuery.dateTime(invChangeDateNo);

		/* statistics */
		stat.itemCount++;
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

		set.add(inventoryChange);
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

@declare@ QueryWithTag
@include@ dbclass.cpp

@declare@ QueryWithoutTag
@include@ dbclass.cpp

@declare@ QueryWare
@include@ dbclass.cpp

@declare@ QueryPartner
@include@ dbclass.cpp

@declare@ Query
@include@ dbclass.cpp

