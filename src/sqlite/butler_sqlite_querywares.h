/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_QUERYWARES_H
#define BUTLER_SQLITE_QUERYWARES_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

namespace Sqlite {

class QueryWaresTable
{
public:
	QueryWaresTable(Sql &_sql);
	virtual ~QueryWaresTable();
private:
	QueryWaresTable();

public:
	void create();
	void check(QStringList &tables);

	void insert(const Query &q, const QString &wareName);
	void del(const Query &q, const QString &wareName);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(const Query &q, WareNameSet &wares);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif


