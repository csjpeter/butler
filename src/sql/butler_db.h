/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DB_H
#define BUTLER_DB_H

#include <butler_sql_connection.h>
#include <butler_tag_db.h>
#include <butler_queries_db.h>
#include <butler_item_db.h>
#include <butler_ware_db.h>
#include <butler_partner_db.h>
#include <butler_company_db.h>
#include <butler_db.h>

class Private;

class Db
{
public:
	Db(const DatabaseDescriptor & dbDesc) :
		sql(dbDesc),
		desc(dbDesc),
		tag(sql),
		query(sql, tag),
		ware(sql, tag),
		partner(sql),
		company(sql),
		item(sql, tag)
	{
		QStringList tables = sql.tables();
		LOG("Tables: %s", C_STR(tables.join(", ")));
		tag.check(tables);
		partner.check(tables);
		company.check(tables);
		ware.check(tables);
		item.check(tables);
		query.check(tables);
	}
	~Db() { }

private:
	SqlConnection sql;
public:
	const DatabaseDescriptor & desc;
	TagDb tag;
	QueryDb query;
	WareDb ware;
	PartnerDb partner;
	CompanyDb company;
	ItemDb item;
};

#endif
