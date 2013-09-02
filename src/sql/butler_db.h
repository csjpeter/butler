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
		desc(dbDesc),
		sql(desc),
		tag(sql),
		query(sql),
		ware(sql),
		company(sql),
		partner(sql),
		item(sql)
	{
	}
	~Db() { }
	bool isOpen() { return sql.isOpen(); }

public:
	DatabaseDescriptor desc;
private:
	SqlConnection sql;
public:
	TagDb tag;
	QueryDb query;
	WareDb ware;
	CompanyDb company;
	PartnerDb partner;
	ItemDb item;
};

#endif
