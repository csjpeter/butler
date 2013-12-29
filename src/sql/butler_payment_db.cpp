/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include "butler_payment_db.h"

#include <math.h>
#include <float.h>

PaymentDb::PaymentDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("payments"))
		sql.exec("CREATE TABLE payments ("
				"uploadDate TIMESTAMP NOT NULL PRIMARY KEY "
						"CHECK('1970-01-01T00:00:00' < uploadDate), "
				"account TEXT NOT NULL, "
				"partner TEXT NOT NULL, "
				"amount DECIMAL(15,3) NOT NULL DEFAULT 0 CHECK(0 <= amount), "
				"comment TEXT NOT NULL DEFAULT '',"
				"lastModified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP "
				")"
				);
	cols = sql.columns("payments");
	if(		!cols.has("account") ||
			!cols.has("partner") ||
			( !cols.has("uploadDate") && !cols.has("uploaddate") ) ||
			!cols.has("amount") ||
			!cols.has("comment") ||
			( !cols.has("lastModified") && !cols.has("lastmodified") )
	  )
		throw DbIncompatibleTableError(
				"Incompatible table payments in the openend database.");

	if(!tables.has("payments_bought"))
		sql.exec("CREATE TABLE payments_bought ("
				  "uploadDate TIMESTAMP NOT NULL REFERENCES Payments(uploadDate) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "purchased TIMESTAMP NOT NULL, "
				  "price DECIMAL(15,2) NOT NULL DEFAULT 0 CHECK(0 <= price), "
				  "partner TEXT NOT NULL REFERENCES partners(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "on_stock INT NOT NULL DEFAULT 0 CHECK(on_stock = 0 OR on_stock = 1)"
				  ")"
			    );
	cols = sql.columns("payments_bought");
	if(		!cols.has("uploadDate") ||
			!cols.has("purchased") ||
			!cols.has("price") ||
			!cols.has("partner") ||
			!cols.has("on_stock")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table payments_bought in the openend database.");
}

PaymentDb::~PaymentDb()
{
}

void PaymentDb::insert(const Payment & i)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO payments (account, partner, uploadDate, "
			"amount, comment) VALUES(?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, i.account);
	sqlQuery.bindValue(1, i.partner);
	sqlQuery.bindValue(2, i.uploadDate);
	sqlQuery.bindValue(3, i.amount);
	sqlQuery.bindValue(4, i.comment);
	sqlQuery.exec();

	if(i.bought){
		sqlQuery.prepare("INSERT INTO payments_bought "
				"(uploadDate, purchased, price, partner, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");
		sqlQuery.bindValue(0, i.uploadDate);
		sqlQuery.bindValue(1, i.purchased);
		sqlQuery.bindValue(2, i.price);
		sqlQuery.bindValue(3, i.partner);
		sqlQuery.bindValue(4, i.onStock ? 1 : 0);
		sqlQuery.exec();
	}

	tr.commit();
}

void PaymentDb::update(const Payment & orig, const Payment & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* The orig and modified object should describe
	 * the same payment's old and new content. */
	if(orig.uploadDate != modified.uploadDate)
		throw DbLogicError("The modified payment is a different payment than the original.");
	sqlQuery.prepare("UPDATE payments SET "
			"account = ?, "
			"partner = ?, "
			"amount = ?, "
			"comment = ? "
			"WHERE uploadDate = ?");
	sqlQuery.bindValue(0, modified.account);
	sqlQuery.bindValue(1, modified.partner);
	sqlQuery.bindValue(2, modified.amount);
	sqlQuery.bindValue(3, modified.comment);
	sqlQuery.bindValue(4, orig.uploadDate);
	sqlQuery.exec();

	if(!orig.bought && modified.bought){
		sqlQuery.prepare("INSERT INTO payments_bought "
				"(uploadDate, purchased, "
				"price, partner, on_stock) "
				"VALUES(?, ?, ?, ?, ?)");
		sqlQuery.bindValue(0, modified.uploadDate);
		sqlQuery.bindValue(1, modified.purchased);
		sqlQuery.bindValue(2, modified.price);
		sqlQuery.bindValue(3, modified.partner);
		sqlQuery.bindValue(4, modified.onStock ? 1 : 0);
		sqlQuery.exec();
	} else if(orig.bought && !modified.bought){
		sqlQuery.prepare("DELETE FROM payments_bought WHERE uploadDate = ?");
		sqlQuery.bindValue(0, orig.uploadDate);
		sqlQuery.exec();
	} else if(orig.bought && modified.bought){
		/* The orig and modified object should describe
		 * the same payment's old and new content. */
		if(orig.uploadDate != modified.uploadDate)
			throw DbLogicError(
					"The modified payment is a different payment than the original.");

		sqlQuery.prepare("UPDATE payments_bought SET "
				"purchased = ?, "
				"price = ?, "
				"partner = ?, "
				"on_stock = ? "
				"WHERE uploadDate = ?");
		sqlQuery.bindValue(0, modified.purchased);
		sqlQuery.bindValue(1, modified.price);
		sqlQuery.bindValue(2, modified.partner);
		sqlQuery.bindValue(3, modified.onStock ? 1 : 0);
		sqlQuery.bindValue(4, orig.uploadDate);
		sqlQuery.exec();
	}

	tr.commit();
}

void PaymentDb::del(const Payment & i)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM payments WHERE uploadDate = ?");
	sqlQuery.bindValue(0, i.uploadDate);
	sqlQuery.exec();

	tr.commit();
}

void PaymentDb::query(const TagNameSet & tags, PaymentSet & payments)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	/* assemble command */
	QString cmd("SELECT "
			" MAX(payments.uploadDate) AS uploadDate,"
			" MAX(payments.account) AS account,"
			" MAX(payments.partner) AS partner,"
			" MAX(payments.comment) AS comment,"
			" MAX(payments.amount) AS amount"
			" FROM payments"
			" LEFT JOIN payments_bought ON payments.uploadDate = payments_bought.uploadDate"
			" LEFT JOIN ware_tags ON payments.account = ware_tags.account"
			" WHERE payments_bought.uploadDate IS NULL");
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
	cmd += " GROUP BY payments.uploadDate ORDER BY payments.uploadDate DESC";
	sqlQuery.exec(cmd);
	payments.clear();
	int uploadDateNo = sqlQuery.colIndex("uploadDate");
	int accountNo = sqlQuery.colIndex("account");
	int partnerNo = sqlQuery.colIndex("partner");
	int amountNo = sqlQuery.colIndex("amount");
	int commentNo = sqlQuery.colIndex("comment");
	DBG("----- Payment query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Payment *payment = new Payment();
		payment->uploadDate = sqlQuery.dateTime(uploadDateNo);
		payment->account = sqlQuery.text(accountNo);
		payment->partner = sqlQuery.text(partnerNo);
		payment->amount = sqlQuery.real(amountNo);
		payment->comment = sqlQuery.text(commentNo);
		payments.add(payment);
	}
	DBG("-----");

	tr.commit();
}

void PaymentDb::query(const Query & q, QueryStat & stat, PaymentSet & payments)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

//	csjp::Time stopper = csjp::Time::unixTime();

	/* assemble command */
	QString cmd("SELECT"
			" MAX(payments.uploadDate) AS uploadDate,"
			" MAX(payments_bought.purchased) AS purchased,"
			" MAX(payments.account) AS account,"
			" MAX(payments.partner) AS partner,"
			" MAX(payments.comment) AS comment,"
			" MAX(payments.amount) AS amount,"
			" MAX(payments_bought.price) AS price,"
			" MAX(payments_bought.partner) AS partner,"
			" MAX(payments_bought.on_stock) AS on_stock"
			" FROM payments_bought"
			" LEFT JOIN payments ON payments_bought.uploadDate = payments.uploadDate"
			" LEFT JOIN ware_tags ON payments.account = ware_tags.account"
			" LEFT JOIN partners ON payments_bought.partner = partners.name");
	
	QString filter;

	if(q.stockOption == Query::StockOptions::PaymentsOnStock){
		if(!filter.isEmpty())
			filter += " AND";
		filter += " on_stock = '1'";
	} else if(q.stockOption == Query::StockOptions::PaymentsUsedUp){
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
			wcmd += " Payments.account = '";
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
		tcmd += " Payments.account NOT IN(SELECT account FROM ware_tags WHERE ";
		for(i=0; i<s; i++){
			if(0 < i)
				tcmd += " OR";
			tcmd += " tag = '";
			tcmd += q.withoutTags.queryAt(i).replace("'", "''");
			tcmd += "'";
		}
		tcmd += " GROUP BY account)";
		if(0 < s)
			filter.append(tcmd);
	}

	if(!filter.isEmpty()){
		cmd += " WHERE";
		cmd += filter;
	}
	
	cmd += " GROUP BY payments.uploadDate";

	if(q.tagOption == Query::TagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd += QString::number(q.withTags.size());
	}

	sqlQuery.exec(cmd);

	payments.clear();

	/* evaluate query result */
	int uploadDateNo = sqlQuery.colIndex("uploadDate");
	int purchasedNo = sqlQuery.colIndex("purchased");
	int accountNo = sqlQuery.colIndex("account");
	int partnerNo = sqlQuery.colIndex("partner");
	int commentNo = sqlQuery.colIndex("comment");
	int amountNo = sqlQuery.colIndex("amount");
	int priceNo = sqlQuery.colIndex("price");
	int partnerNo = sqlQuery.colIndex("partner");
	int onStockNo = sqlQuery.colIndex("on_stock");

	/* statistics */
	stat.paymentCount = 0;
	stat.sumQuantity = 0;
	stat.sumPrice = 0;
	stat.cheapestUnitPrice = DBL_MAX;
	stat.mostExpUnitPrice = 0;
	double sumPrice = 0;
	double sumQuantity = 0;

	DBG("----- Payment query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Payment *payment = new Payment();
		payment->uploadDate = sqlQuery.dateTime(uploadDateNo);
		payment->purchased = sqlQuery.dateTime(purchasedNo);
		payment->account = sqlQuery.text(accountNo);
		payment->partner = sqlQuery.text(partnerNo);
		payment->comment = sqlQuery.text(commentNo);
		payment->amount = sqlQuery.real(amountNo);
		payment->price = sqlQuery.real(priceNo);
		payment->partner = sqlQuery.text(partnerNo);
		payment->onStock = sqlQuery.number(onStockNo);
		payment->bought = true;

		/* statistics */
		stat.paymentCount++;
		stat.sumQuantity += payment->amount;
		stat.sumPrice += payment->price;
		if(DBL_EPSILON <= payment->amount && DBL_EPSILON <= payment->price){
			sumQuantity += payment->amount;
			sumPrice += payment->price;

			double unitPrice = payment->price / payment->amount;
			if(unitPrice < stat.cheapestUnitPrice)
				stat.cheapestUnitPrice = unitPrice;
			if(stat.mostExpUnitPrice < unitPrice)
				stat.mostExpUnitPrice = unitPrice;
		}

		payments.add(payment);
	}

	stat.avgPrice = sumPrice / sumQuantity;
	if(!stat.paymentCount){
		stat.cheapestUnitPrice = NAN;
		stat.mostExpUnitPrice = NAN;
	}

	DBG("-----");
//	stat.queryTime = stopper - csjp::Time::unixTime();

	tr.commit();
}

/*
select * from (select Payments.uploadDate as id, Payments.name as ware, Payments.type as type, payments_bought.purchased as bought, payments_bought.price as price from Payments left join payments_bought on Payments.uploadDate = payments_bought.uploadDate left join WareTags on Payments.name = WareTags.name where '2012-02-01T00:00:00' < payments_bought.purchased and payments_bought.purchased < '2012-03-01T00:00:00' and WareTags.tag = 'élelmiszer' group by Payments.uploadDate);
*/
