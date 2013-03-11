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

#include "butler_item_table.h"

ItemTable::ItemTable(SqlConnection &_sql) :
	sql(_sql)
{
	DBG("sql: %p", &sql);
}

ItemTable::~ItemTable()
{
}

void ItemTable::check(QStringList &tables)
{
	if(!tables.contains("Items"))
		sql.exec("CREATE TABLE Items ("
				  "uploaded DATE NOT NULL PRIMARY KEY CHECK('1970-01-01T00:00:00' < uploaded), "
				  "name VARCHAR(256) NOT NULL, "
				  "category VARCHAR(256) NOT NULL, "
				  "quantity REAL NOT NULL DEFAULT 0 CHECK(0 <= quantity), "
				  "comment TEXT NOT NULL DEFAULT ''"
				  ")"
				  );

	QSqlRecord table = sql.record("Items");
	if(		!table.contains("name") ||
			!table.contains("category") ||
			!table.contains("uploaded") ||
			!table.contains("quantity") ||
			!table.contains("comment")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table Items in the openend database.");
}

void ItemTable::insert(const Item &i)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO Items "
				"(name, category, uploaded, "
				"quantity, comment) "
				"VALUES(?, ?, ?, ?, ?)");

	insertQuery.bindValue(0, i.name);
	insertQuery.bindValue(1, i.category);
	insertQuery.bindValue(2, i.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	insertQuery.bindValue(3, i.quantity);
	insertQuery.bindValue(4, i.comment);
	insertQuery.exec();
}

void ItemTable::update(const Item &orig, const Item &modified)
{
	SqlQuery updateQuery(sql);
	/* The orig and modified object should describe
	 * the same item's old and new content. */
	if(orig.uploaded.toString() != modified.uploaded.toString())
		throw DbLogicError("The modified item is a different item than the original.");

	if(!updateQuery.isPrepared())
		updateQuery.prepare("UPDATE Items SET "
				"name = ?, "
				"category = ?, "
				"quantity = ?, "
				"comment = ? "
				"WHERE uploaded = ?");

	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, modified.category);
	updateQuery.bindValue(2, modified.quantity);
	updateQuery.bindValue(3, modified.comment);
	updateQuery.bindValue(4, orig.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	updateQuery.exec();
}

void ItemTable::del(const Item &i)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM Items WHERE "
				"uploaded = ?");

	deleteQuery.bindValue(0, i.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	deleteQuery.exec();
}

void ItemTable::query(Item& item)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT * FROM Items "
				"WHERE uploaded = ?");

	selectQuery.bindValue(0, item.uploaded.toUTC().toString("yyyy-MM-ddThh:mm:ss"));
	selectQuery.exec();

	int nameNo = selectQuery.colIndex("name");
	int categoryNo = selectQuery.colIndex("category");
	int quantityNo = selectQuery.colIndex("quantity");
	int commentNo = selectQuery.colIndex("comment");

	DBG("----- Item query result:");
	if(selectQuery.next()) {
		item.name = selectQuery.value(nameNo).toString();
		item.category = selectQuery.value(categoryNo).toString();
		item.quantity = selectQuery.value(quantityNo).toDouble();
		item.comment = selectQuery.value(commentNo).toString();
	}
	DBG("-----");
}

void ItemTable::query(const TagNameSet &tags, ItemSet &items)
{
	SqlQuery sqlQuery(sql);

	/* assemble command */
	QString cmd("SELECT * FROM Items"
			" LEFT JOIN ItemsBought ON Items.uploaded = ItemsBought.uploaded"
			" LEFT JOIN WareTags ON Items.name = WareTags.name"
			" WHERE ItemsBought.uploaded IS NULL");
	{
		unsigned i, s = tags.size();
		if(s)
			cmd += " AND (";
		for(i=0; i<s; i++){
			if(0 < i)
				cmd += " OR";
			cmd += " tag = '";
			/* FIXME need escapeing here */
			cmd += tags.queryAt(i);
			cmd += "'";
		}
		if(0 < s)
			cmd += ")";
	}
	
	cmd += " GROUP BY Items.uploaded ORDER BY Items.uploaded DESC";

	DBG("Assembled select query: %s", qPrintable(cmd));
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
}
