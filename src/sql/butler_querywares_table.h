/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYWARES_TABLE_H
#define BUTLER_QUERYWARES_TABLE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class QueryWaresTable
{
public:
	QueryWaresTable(SqlConnection &_sql);
	virtual ~QueryWaresTable();
private:
	QueryWaresTable();

public:
	void check(QStringList &tables);

	void insert(const Query &q, const QString &wareName);
	void del(const Query &q, const QString &wareName);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(const Query &q, WareNameSet &wares);

private:
	SqlConnection &sql;
};

#endif
