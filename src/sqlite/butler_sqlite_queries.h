/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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
	bool create();
	bool check(QStringList &tables);

	bool insert(const Query &qo);
	bool update(const Query &orig,
			const Query &modified);
	bool del(const Query &qo);
	bool query(const QString &name, Query &query);
	bool query(QuerySet &queries);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectAllQuery;
};

}

#endif

