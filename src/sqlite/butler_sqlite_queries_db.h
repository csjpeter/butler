/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_QUERY_DB_H
#define BUTLER_SQLITE_QUERY_DB_H

#include <butler_db.h>

#include "butler_sqlite_queries.h"
#include "butler_sqlite_tag_db.h"

namespace Sqlite {

class QueryDb : public ::QueryDb
{
public:
	QueryDb(Sql &sql, TagDb &tagDb);
	virtual ~QueryDb();
private:
	QueryDb();

public:
	bool create();
	bool check(QStringList &tables);
	bool update();

	bool insert(const Query &q);
	bool update(const Query &orig, const Query &modified);
	bool del(const Query &q);
	bool query(QuerySet &qs);

private:
	Sql &sql;
	TagDb &tagDb;
	QueryTable queryTable;
	QueryTagsTable queryTagsTable;
	QueryWaresTable queryWaresTable;
	QueryShopsTable queryShopsTable;
};

}

#endif

