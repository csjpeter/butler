/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_INVENTORY_CHANGE_DB_H
#define BUTLER_INVENTORY_CHANGE_DB_H

#include <butler_sql_connection.h>

class InventoryChangeDb
{
public:
	InventoryChangeDb(SqlConnection & sql);
	virtual ~InventoryChangeDb();
private:
	InventoryChangeDb();

public:
	void insert(const InventoryChange &);
	void update(const InventoryChange & orig, const InventoryChange & modified);
	void del(const InventoryChange &);
	void query(const TagNameSet &, InventoryChangeSet &);
	void query(const Query &, QueryStat &, InventoryChangeSet &);

private:
	SqlConnection & sql;
};

#endif
