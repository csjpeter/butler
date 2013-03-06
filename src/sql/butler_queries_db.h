/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_QUERY_DB_H
#define BUTLER_SQLITE_QUERY_DB_H

#include <butler_db.h>

#include "butler_sqlite_queries.h"
#include "butler_sqlite_tag_db.h"

class QueryDb : public ::QueryDb
{
public:
	QueryDb(SqlConnection &sql, TagDb &tagDb);
	virtual ~QueryDb();
private:
	QueryDb();

public:
	void check(QStringList &tables);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(QuerySet &qs);

private:
	SqlConnection &sql;
	TagDb &tagDb;
	QueryTable queryTable;
	QueryTagsTable queryTagsTable;
	QueryWaresTable queryWaresTable;
	QueryShopsTable queryShopsTable;
};

#endif
