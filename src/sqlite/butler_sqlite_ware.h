/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_WARE_H
#define BUTLER_SQLITE_WARE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

#include "butler_sqlite_waretags.h"

namespace Sqlite {

class WareTable
{
public:
	WareTable(Sql &sql);
	virtual ~WareTable();
private:
	WareTable();

public:
	bool create();
	bool check(QStringList &tables);
	bool insert(const Ware &);
	bool update(const Ware &orig,
			const Ware &modified);
	bool del(const Ware &);
	bool query(WareSet &);

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
