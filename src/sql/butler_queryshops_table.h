/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYSHOPS_H
#define BUTLER_QUERYSHOPS_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class QueryShopsTable
{
public:
	QueryShopsTable(SqlConnection &_sql);
	virtual ~QueryShopsTable();
private:
	QueryShopsTable();

public:
	void check(QStringList &tables);

	void insert(const Query &q, const QString &shopName);
	void del(const Query &q, const QString &shopName);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(const Query &q, ShopNameSet &shops);

private:
	SqlConnection &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

#endif
