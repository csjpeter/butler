/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_DB_H
#define BUTLER_WARE_DB_H

#include <butler_ware_table.h>
#include <butler_waretags_table.h>
#include <butler_warecategories_table.h>
#include <butler_tag_db.h>

class WareDb
{
public:
	WareDb(SqlConnection &sql, TagDb &tagDb);
	virtual ~WareDb();
private:
	WareDb();

public:
	void check(QStringList &tables);

	void insert(const Ware &);
	void update(const Ware &orig, const Ware &modified);
	void del(const Ware &);
	void query(WareSet &);

private:
	SqlConnection &sql;
	TagDb &tagDb;
	WareTable wareTable;
	WareTagsTable wareTagsTable;
	WareCategoriesTable wareCategoriesTable;
};

#endif
