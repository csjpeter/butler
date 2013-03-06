/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_WARE_DB_H
#define BUTLER_SQLITE_WARE_DB_H

#include <butler_db.h>

#include "butler_sqlite_ware.h"
#include "butler_sqlite_waretags.h"
#include "butler_sqlite_warecategories.h"
#include "butler_sqlite_tag_db.h"

namespace Sqlite {

class WareDb : public ::WareDb
{
public:
	WareDb(Sql &sql, TagDb &tagDb);
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
	Sql &sql;
	TagDb &tagDb;
	WareTable wareTable;
	WareTagsTable wareTagsTable;
	WareCategoriesTable wareCategoriesTable;
};

}

#endif
