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

class WareTable
{
public:
	WareTable(SqlConnection &sql);
	virtual ~WareTable();
private:
	WareTable();

public:
	void check(QStringList &tables);
	void insert(const Ware &);
	void update(const Ware &orig,
			const Ware &modified);
	void del(const Ware &);
	void query(WareSet &);

private:
	SqlConnection &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
	SqlQuery selectAllQuery;
};

#endif
