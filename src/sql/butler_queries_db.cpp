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
				"end_date TIMESTAMP NOT NULL, "
				"lastModified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP "
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
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	if(!sqlQuery.isPrepared())
		sqlQuery.prepare("INSERT INTO queries "
			"(query_name, stock_option, tag_option, start_date, end_date) "
			"VALUES (?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, query.name);
	sqlQuery.bindValue(1, (int)query.stockOption);
	sqlQuery.bindValue(2, (int)query.tagOption);
	sqlQuery.bindValue(3, query.startDate);
	sqlQuery.bindValue(4, query.endDate);
	sqlQuery.exec();

	unsigned i, s = query.partners.size();
	sqlQuery.prepare("INSERT INTO query_partners (query_name, partner) VALUES (?, ?)");
	for(i=0; i<s; i++){
		const Text & partnerName = query.partners.queryAt(i);
		sqlQuery.bindValue(0, query.name);
		sqlQuery.bindValue(1, partnerName);
		sqlQuery.exec();
	}

	s = query.wares.size();
	sqlQuery.prepare("INSERT INTO query_wares (query_name, ware) VALUES (?, ?)");
	for(i=0; i<s; i++){
		const Text & wareName = query.wares.queryAt(i);
		sqlQuery.bindValue(0, query.name);
		sqlQuery.bindValue(1, wareName);
		sqlQuery.exec();
	}

	s = query.withTags.size();
	sqlQuery.prepare("INSERT INTO query_tags (query_name, tag) VALUES (?, ?)");
	for(i=0; i<s; i++){
		const Text & tagName = query.withTags.queryAt(i);
		sqlQuery.bindValue(0, query.name);
		sqlQuery.bindValue(1, tagName);
		sqlQuery.exec();
	}

	tr.commit();
}

void QueryDb::update(const Query & orig, const Query & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE queries SET "
			"query_name = ?, "
			"stock_option = ?, "
			"tag_option = ?, "
			"start_date = ?, "
			"end_date = ? "
			"WHERE query_name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, (int)modified.stockOption);
	sqlQuery.bindValue(2, (int)modified.tagOption);
	sqlQuery.bindValue(3, modified.startDate);
	sqlQuery.bindValue(4, modified.endDate);
	sqlQuery.bindValue(5, orig.name);
	sqlQuery.exec();

	unsigned i, s = modified.partners.size();
	sqlQuery.prepare("INSERT INTO query_partners (query_name, partner) VALUES (?, ?)");
	for(i=0; i<s; i++){
		const Text & partnerName = modified.partners.queryAt(i);
		if(orig.partners.has(partnerName))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, partnerName);
		sqlQuery.exec();
	}
	s = orig.partners.size();
	sqlQuery.prepare("DELETE FROM query_partners WHERE query_name = ? AND partner = ?");
	for(i=0; i<s; i++){
		const Text & partnerName = orig.partners.queryAt(i);
		if(modified.partners.has(partnerName))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, partnerName);
		sqlQuery.exec();
	}

	s = modified.wares.size();
	sqlQuery.prepare("INSERT INTO query_wares (query_name, ware) VALUES (?, ?)");
	for(i=0; i<s; i++){
		const Text & wareName = modified.wares.queryAt(i);
		if(orig.wares.has(wareName))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, wareName);
		sqlQuery.exec();
	}
	s = orig.wares.size();
	sqlQuery.prepare("DELETE FROM query_wares WHERE query_name = ? AND ware = ?");
	for(i=0; i<s; i++){
		const Text & wareName = orig.wares.queryAt(i);
		if(modified.wares.has(wareName))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, wareName);
		sqlQuery.exec();
	}

	s = modified.withTags.size();
	sqlQuery.prepare("INSERT INTO query_tags (query_name, tag) VALUES (?, ?)");
	for(i=0; i<s; i++){
		const Text & tagName = modified.withTags.queryAt(i);
		if(orig.withTags.has(tagName))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, tagName);
		sqlQuery.exec();
	}
	s = orig.withTags.size();
	sqlQuery.prepare("DELETE FROM query_tags WHERE query_name = ? AND tag = ?");
	for(i=0; i<s; i++){
		const Text & tagName = orig.withTags.queryAt(i);
		if(modified.withTags.has(tagName))
			continue;
		sqlQuery.bindValue(0, modified.name);
		sqlQuery.bindValue(1, tagName);
		sqlQuery.exec();
	}

	tr.commit();
}

void QueryDb::del(const Query & query)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM queries WHERE query_name = ?");
	sqlQuery.bindValue(0, query.name);
	sqlQuery.exec();

	tr.commit();
}

void QueryDb::query(QuerySet & queries)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT query_name, stock_option, tag_option, start_date, end_date "
		"FROM queries");
	sqlQuery.exec();
	queries.clear();
	int queryNameNo = sqlQuery.colIndex("query_name");
	int stockOptionNo = sqlQuery.colIndex("stock_option");
	int tagOptionNo = sqlQuery.colIndex("tag_option");
	int startDateNo = sqlQuery.colIndex("start_date");
	int endDateNo = sqlQuery.colIndex("end_date");
	DBG("----- Query query result:");
	while (sqlQuery.next()) {
		DBG("Next row");
		Query *query = new Query();
		query->name = sqlQuery.text(queryNameNo);
		query->stockOption = (enum Query::StockOptions)sqlQuery.number(stockOptionNo);
		query->tagOption = (enum Query::TagOptions)sqlQuery.number(tagOptionNo);
		query->startDate = sqlQuery.dateTime(startDateNo);
		query->endDate = sqlQuery.dateTime(endDateNo);

		queries.add(query);
	}
	DBG("-----");

	unsigned s = queries.size();
	for(unsigned i=0; i<s; i++){
		Query & query = queries.queryAt(i);

		sqlQuery.prepare("SELECT query_name, partner FROM query_partners "
				"WHERE query_name = ?");
		sqlQuery.bindValue(0, query.name);
		sqlQuery.exec();
		query.partners.clear();
		int partnerNo = sqlQuery.colIndex("partner");
		DBG("----- Query partners query result:");
		while (sqlQuery.next()) {
			DBG("Next row");
			query.partners.add(new Text(sqlQuery.text(partnerNo)));
		}
		DBG("-----");

		sqlQuery.prepare("SELECT query_name, ware FROM query_wares WHERE query_name = ?");
		sqlQuery.bindValue(0, query.name);
		sqlQuery.exec();
		query.wares.clear();
		int wareNo = sqlQuery.colIndex("ware");
		DBG("----- Query wares query result:");
		while (sqlQuery.next()) {
			DBG("Next row");
			query.wares.add(new Text(sqlQuery.text(wareNo)));
		}
		DBG("-----");

		sqlQuery.prepare("SELECT query_name, tag FROM query_tags WHERE query_name = ?");
		sqlQuery.bindValue(0, query.name);
		sqlQuery.exec();
		query.withTags.clear();
		int tagNo = sqlQuery.colIndex("tag");
		DBG("----- Query withTags query result:");
		while (sqlQuery.next()) {
			DBG("Next row");
			query.withTags.add(new Text(sqlQuery.text(tagNo)));
		}
		DBG("-----");

		/* FIXME : We need to upgrade the schema for this. */
		/* queryTagsTable.query(query, query.withoutTags); */
	}

	tr.commit();

}
