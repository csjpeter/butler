/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERY_DB_H
#define BUTLER_QUERY_DB_H

#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class QueryDb
{
public:
	QueryDb(SqlConnection & sql);
	virtual ~QueryDb();
private:
	QueryDb();

public:
	void insert(const Query & query);
	void update(const Query & orig, const Query & modified);
	void del(const Query & query);
	void query(QuerySet & queries);

private:
	SqlConnection & sql;
};

#endif
