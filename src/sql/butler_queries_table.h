/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERIES_TABLE_H
#define BUTLER_QUERIES_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_sql_connection.h>

#include <butler_query_set.h>

class QueryTable
{
public:
	QueryTable(SqlConnection &sql);
	virtual ~QueryTable();
private:
	QueryTable();

public:
	void check(QStringList &tables);

	void insert(const Query &qo);
	void update(const Query &orig, const Query &modified);
	void del(const Query &qo);
	void query(const QString &name, Query &query);
	void query(QuerySet &queries);

private:
	SqlConnection &sql;
};

#endif
