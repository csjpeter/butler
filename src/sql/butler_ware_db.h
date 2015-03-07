/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_DB_H
#define BUTLER_WARE_DB_H
#if 0
#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class WareDb
{
public:
	WareDb(SqlConnection & sql);
	virtual ~WareDb();
private:
	WareDb();

public:
	void insert(const Ware &);
	void update(const Ware & orig, const Ware & modified);
	void del(const Ware &);
	void query(WareSet &);

private:
	SqlConnection & sql;
};
#endif
#endif
