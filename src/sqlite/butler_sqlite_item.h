/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_ITEM_H
#define BUTLER_SQLITE_ITEM_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

namespace Sqlite {

class ItemTable
{
public:
	ItemTable(Sql &sql);
	virtual ~ItemTable();
private:
	ItemTable();

public:
	void create();
	void check(QStringList &tables);
	void insert(const Item &);
	void update(const Item &orig,
			const Item &modified);
	void del(const Item &);
	void query(Item &);
	/* results only with unbought items */
	void query(const TagNameSet &, ItemSet &);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
	SqlQuery selectNamesQuery;
};

}

#endif

