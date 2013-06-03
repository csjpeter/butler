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

#include <butler_query.h>

#include "butler_querypartners_table.h"

QueryPartnersTable::QueryPartnersTable(SqlConnection &_sql) :
	sql(_sql)
{
}

QueryPartnersTable::~QueryPartnersTable()
{
}


void QueryPartnersTable::check(QStringList &tables)
{
	if(!tables.contains("query_partners")){
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

	QSqlRecord table = sql.record("query_partners");
	if(		!table.contains("query_name") ||
			!table.contains("partner")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table query_partners in the openend database.");
}

void QueryPartnersTable::insert(const Query &q, const QString &partnerName)
{
	SqlQuery insertQuery(sql);
	if(!insertQuery.isPrepared())
		insertQuery.prepare("INSERT INTO query_partners "
				"(query_name, partner) "
				"VALUES (?, ?)");

	insertQuery.bindValue(0, q.name);
	insertQuery.bindValue(1, partnerName);
	insertQuery.exec();
}

void QueryPartnersTable::del(const Query &q, const QString &partnerName)
{
	SqlQuery deleteQuery(sql);
	if(!deleteQuery.isPrepared())
		deleteQuery.prepare(
				"DELETE FROM query_partners WHERE "
				"query_name = ? AND partner = ?");

	deleteQuery.bindValue(0, q.name);
	deleteQuery.bindValue(1, partnerName);
	deleteQuery.exec();
}

void QueryPartnersTable::insert(const Query &q)
{
	unsigned i, s = q.partners.size();
	for(i=0; i<s; i++){
		const QString &t = q.partners.queryAt(i);
		insert(q, t);
	}
}

void QueryPartnersTable::update(const Query &orig, const Query &modified)
{
	unsigned i, s = modified.partners.size();
	for(i=0; i<s; i++){
		const QString &mTag = modified.partners.queryAt(i);
		if(!orig.partners.has(mTag))
			insert(modified, mTag);
	}

	s = orig.partners.size();
	for(i=0; i<s; i++){
		const QString &oTag = orig.partners.queryAt(i);
		if(!modified.partners.has(oTag))
			del(orig, oTag);
	}
}

void QueryPartnersTable::query(const Query &q, PartnerNameSet &partners)
{
	SqlQuery selectQuery(sql);
	if(!selectQuery.isPrepared())
		selectQuery.prepare("SELECT query_name, partner FROM query_partners "
				"WHERE query_name = ?");

	selectQuery.bindValue(0, q.name);
	selectQuery.exec();

	partners.clear();

	int partnerNo = selectQuery.colIndex("partner");

	DBG("----- Query partners query result:");
	while (selectQuery.next()) {
		DBG("Next row");
		partners.add(new QString(selectQuery.value(partnerNo).toString()));
	}
	DBG("-----");
}
