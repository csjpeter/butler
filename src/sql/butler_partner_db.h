/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_DB_H
#define BUTLER_PARTNER_DB_H

#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class PartnerDb
{
public:
	PartnerDb(SqlConnection & sql);
	virtual ~PartnerDb();
private:
	PartnerDb();

public:
	void insert(const Partner &);
	void update(const Partner & orig, const Partner & modified);
	void del(const Partner &);
	void query(PartnerSet &);

private:
	SqlConnection & sql;
};

#endif
