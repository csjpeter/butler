/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_INVENTORY_DB_H
#define BUTLER_INVENTORY_DB_H
#if 0

#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class InventoryDb
{
public:
	InventoryDb(SqlConnection & sql);
	virtual ~InventoryDb();
private:
	InventoryDb();

public:
	void insert(const Inventory &);
	void update(const Inventory & orig, const Inventory & modified);
	void del(const Inventory &);
	void query(InventorySet &);

private:
	SqlConnection & sql;
};

#endif
#endif
