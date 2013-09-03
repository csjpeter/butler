/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_queries_db.h"

QueryDb::QueryDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("queries"))
		sql.exec("CREATE TABLE queries ("
				  "query_name VARCHAR(64) NOT NULL PRIMARY KEY, "
				  "stock_option INTEGER NOT NULL, "
				  "tag_option INTEGER NOT NULL, "
				  "start_date TIMESTAMP NOT NULL, "
				  "end_date TIMESTAMP NOT NULL"
				  ")"
			    );
	cols = sql.columns("queries");
	if(		!cols.has("query_name") ||
			!cols.has("stock_option") ||
			!cols.has("tag_option") ||
			!cols.has("start_date") ||
			!cols.has("end_date")
	  )
		throw DbIncompatibleTableError(
				"Incompatible table queries in the openend database.");

	if(!tables.has("query_partners")){
		sql.exec("CREATE TABLE query_partners ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES Queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "partner VARCHAR(64) NOT NULL REFERENCES Partners(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, partner)"
				  ")"
			    );
		sql.exec("CREATE INDEX query_partners_query_name_index ON query_partners(query_name)");
		sql.exec("CREATE INDEX query_partners_tag_index ON query_partners(partner)");
	}
	cols = sql.columns("query_partners");
	if(		!cols.has("query_name") ||
			!cols.has("partner")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table query_partners in the openend database.");

	if(!tables.has("query_wares")){
		sql.exec("CREATE TABLE query_wares ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "ware VARCHAR(64) NOT NULL REFERENCES wares(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, ware)"
				  ")"
			    );
		sql.exec("CREATE INDEX query_wares_query_name_index ON query_wares(query_name)");
		sql.exec("CREATE INDEX query_wares_tag_index ON query_wares(ware)");
	}
	cols = sql.columns("query_wares");
	if(		!cols.has("query_name") ||
			!cols.has("ware")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table query_wares in the openend database.");

	if(!tables.has("query_tags")){
		sql.exec("CREATE TABLE query_tags ("
				  "query_name VARCHAR(64) NOT NULL REFERENCES queries(query_name) "
				  "ON DELETE CASCADE ON UPDATE CASCADE, "
				  "tag VARCHAR(64) NOT NULL REFERENCES tags(name) "
				  "ON DELETE RESTRICT ON UPDATE CASCADE, "
				  "UNIQUE (query_name, tag)"
				  ")"
			    );
		sql.exec("CREATE INDEX query_tags_query_name_index ON query_tags(query_name)");
		sql.exec("CREATE INDEX query_tags_query_tag_index ON query_tags(tag)");
	}
	cols = sql.columns("query_tags");
	if(		!cols.has("query_name") ||
			!cols.has("tag")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table query_tags in the openend database.");
}

QueryDb::~QueryDb()
{
}

void QueryDb::insert(const Query & query)
{
	SqlTransaction tr(sql);

	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO queries "
			"(query_name, stock_option, tag_option, start_date, end_date) "
			"VALUES (?, ?, ?, ?, ?)");
	insertQuery.bindValue(0, query.name);
	insertQuery.bindValue(1, (int)query.stockOption);
	insertQuery.bindValue(2, (int)query.tagOption);
	insertQuery.bindValue(3, query.startDate);
	insertQuery.bindValue(4, query.endDate);
	insertQuery.exec();

	unsigned i, s = query.partners.size();
	for(i=0; i<s; i++){
		const Text & partnerName = query.partners.queryAt(i);
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO query_partners "
					"(query_name, partner) VALUES (?, ?)");
		insertQuery.bindValue(0, query.name);
		insertQuery.bindValue(1, partnerName);
		insertQuery.exec();
	}

	s = query.wares.size();
	for(i=0; i<s; i++){
		const Text & wareName = query.wares.queryAt(i);
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO query_wares (query_name, ware) VALUES (?, ?)");
		insertQuery.bindValue(0, query.name);
		insertQuery.bindValue(1, wareName);
		insertQuery.exec();
	}

	s = query.withTags.size();
	for(i=0; i<s; i++){
		const Text & tagName = query.withTags.queryAt(i);
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO query_tags (query_name, tag) VALUES (?, ?)");
		insertQuery.bindValue(0, query.name);
		insertQuery.bindValue(1, tagName);
		insertQuery.exec();
	}

	tr.commit();
}

void QueryDb::update(const Query & orig, const Query & modified)
{
	SqlTransaction tr(sql);

	SqlQuery updateQuery(sql);
	updateQuery.prepare("UPDATE queries SET "
			"query_name = ?, "
			"stock_option = ?, "
			"tag_option = ?, "
			"start_date = ?, "
			"end_date = ? "
			"WHERE query_name = ?");
	updateQuery.bindValue(0, modified.name);
	updateQuery.bindValue(1, (int)modified.stockOption);
	updateQuery.bindValue(2, (int)modified.tagOption);
	updateQuery.bindValue(3, modified.startDate);
	updateQuery.bindValue(4, modified.endDate);
	updateQuery.bindValue(5, orig.name);
	updateQuery.exec();

	unsigned i, s = modified.partners.size();
	for(i=0; i<s; i++){
		const Text & partnerName = modified.partners.queryAt(i);
		if(orig.partners.has(partnerName))
			continue;
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO query_partners "
					"(query_name, partner) VALUES (?, ?)");
		insertQuery.bindValue(0, modified.name);
		insertQuery.bindValue(1, partnerName);
		insertQuery.exec();
	}
	s = orig.partners.size();
	for(i=0; i<s; i++){
		const Text & partnerName = orig.partners.queryAt(i);
		if(modified.partners.has(partnerName))
			continue;
		SqlQuery deleteQuery(sql);
		deleteQuery.prepare(
				"DELETE FROM query_partners WHERE query_name = ? AND partner = ?");
		deleteQuery.bindValue(0, modified.name);
		deleteQuery.bindValue(1, partnerName);
		deleteQuery.exec();
	}

	s = modified.wares.size();
	for(i=0; i<s; i++){
		const Text & wareName = modified.wares.queryAt(i);
		if(orig.wares.has(wareName))
			continue;
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO query_wares (query_name, ware) VALUES (?, ?)");
		insertQuery.bindValue(0, modified.name);
		insertQuery.bindValue(1, wareName);
		insertQuery.exec();
	}
	s = orig.wares.size();
	for(i=0; i<s; i++){
		const Text & wareName = orig.wares.queryAt(i);
		if(modified.wares.has(wareName))
			continue;
		SqlQuery deleteQuery(sql);
		deleteQuery.prepare("DELETE FROM query_wares WHERE query_name = ? AND ware = ?");
		deleteQuery.bindValue(0, modified.name);
		deleteQuery.bindValue(1, wareName);
		deleteQuery.exec();
	}

	s = modified.withTags.size();
	for(i=0; i<s; i++){
		const Text & tagName = modified.withTags.queryAt(i);
		if(orig.withTags.has(tagName))
			continue;
		SqlQuery insertQuery(sql);
		insertQuery.prepare("INSERT INTO query_tags (query_name, tag) VALUES (?, ?)");
		insertQuery.bindValue(0, modified.name);
		insertQuery.bindValue(1, tagName);
		insertQuery.exec();
	}
	s = orig.withTags.size();
	for(i=0; i<s; i++){
		const Text & tagName = orig.withTags.queryAt(i);
		if(modified.withTags.has(tagName))
			continue;
		SqlQuery deleteQuery(sql);
		deleteQuery.prepare("DELETE FROM query_tags WHERE query_name = ? AND tag = ?");
		deleteQuery.bindValue(0, modified.name);
		deleteQuery.bindValue(1, tagName);
		deleteQuery.exec();
	}

	tr.commit();
}

void QueryDb::del(const Query & query)
{
	SqlTransaction tr(sql);

	SqlQuery deleteQuery(sql);
	deleteQuery.prepare("DELETE FROM queries WHERE query_name = ?");
	deleteQuery.bindValue(0, query.name);
	deleteQuery.exec();

	tr.commit();
}

void QueryDb::query(QuerySet & queries)
{
	SqlTransaction tr(sql);

	SqlQuery selectAllQuery(sql);
	selectAllQuery.prepare("SELECT query_name, stock_option, tag_option, start_date, end_date "
		"FROM queries");
	selectAllQuery.exec();
	queries.clear();
	int queryNameNo = selectAllQuery.colIndex("query_name");
	int stockOptionNo = selectAllQuery.colIndex("stock_option");
	int tagOptionNo = selectAllQuery.colIndex("tag_option");
	int startDateNo = selectAllQuery.colIndex("start_date");
	int endDateNo = selectAllQuery.colIndex("end_date");
	DBG("----- Query query result:");
	while (selectAllQuery.next()) {
		DBG("Next row");
		Query *query = new Query();
		query->name = selectAllQuery.text(queryNameNo);
		query->stockOption = (enum Query::StockOptions)selectAllQuery.number(stockOptionNo);
		query->tagOption = (enum Query::TagOptions)selectAllQuery.number(tagOptionNo);
		query->startDate = selectAllQuery.dateTime(startDateNo);
		query->endDate = selectAllQuery.dateTime(endDateNo);

		queries.add(query);
	}
	DBG("-----");

	unsigned s = queries.size();
	for(unsigned i=0; i<s; i++){
		Query & query = queries.queryAt(i);

		SqlQuery selectQuery(sql);
		selectQuery.prepare("SELECT query_name, partner FROM query_partners "
				"WHERE query_name = ?");
		selectQuery.bindValue(0, query.name);
		selectQuery.exec();
		query.partners.clear();
		int partnerNo = selectQuery.colIndex("partner");
		DBG("----- Query partners query result:");
		while (selectQuery.next()) {
			DBG("Next row");
			query.partners.add(new Text(selectQuery.text(partnerNo)));
		}
		DBG("-----");

		SqlQuery selectWaresQuery(sql);
		selectWaresQuery.prepare(
				"SELECT query_name, ware FROM query_wares WHERE query_name = ?");
		selectWaresQuery.bindValue(0, query.name);
		selectWaresQuery.exec();
		query.wares.clear();
		int wareNo = selectWaresQuery.colIndex("ware");
		DBG("----- Query wares query result:");
		while (selectWaresQuery.next()) {
			DBG("Next row");
			query.wares.add(new Text(selectWaresQuery.text(wareNo)));
		}
		DBG("-----");

		SqlQuery selectTagsQuery(sql);
		selectTagsQuery.prepare(
				"SELECT query_name, tag FROM query_tags WHERE query_name = ?");
		selectTagsQuery.bindValue(0, query.name);
		selectTagsQuery.exec();
		query.withTags.clear();
		int tagNo = selectTagsQuery.colIndex("tag");
		DBG("----- Query withTags query result:");
		while (selectTagsQuery.next()) {
			DBG("Next row");
			query.withTags.add(new Text(selectTagsQuery.text(tagNo)));
		}
		DBG("-----");

		/* FIXME : We need to upgrade the schema for this. */
		/* queryTagsTable.query(query, query.withoutTags); */
	}

	tr.commit();

}
