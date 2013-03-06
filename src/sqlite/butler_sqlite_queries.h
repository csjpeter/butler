/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_QUERIES_H
#define BUTLER_SQLITE_QUERIES_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

#include "butler_sqlite_querytags.h"
#include "butler_sqlite_querywares.h"
#include "butler_sqlite_queryshops.h"

namespace Sqlite {

class QueryTable
{
public:
	QueryTable(Sql &sql);
	virtual ~QueryTable();
private:
	QueryTable();

public:
	void create();
	void check(QStringList &tables);

	void insert(const Query &qo);
	void update(const Query &orig,
			const Query &modified);
	void del(const Query &qo);
	void query(const QString &name, Query &query);
	void query(QuerySet &queries);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectAllQuery;
};

}

#endif

