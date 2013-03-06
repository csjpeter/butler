/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_ITEMBOUGHT_H
#define BUTLER_SQLITE_ITEMBOUGHT_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

namespace Sqlite {

class ItemBoughtTable
{
public:
	ItemBoughtTable(Sql &sql);
	virtual ~ItemBoughtTable();
private:
	ItemBoughtTable();

public:
	void check(QStringList &tables);
	void insert(const Item &i);
	void update(const Item &orig,
			const Item &modified);
	void del(const Item &item);
	void query(const Query &q, QueryStat &stat, ItemSet &items);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
	SqlQuery selectAllQuery;
};

}

#endif

