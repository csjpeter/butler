/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_TAG_H
#define BUTLER_SQLITE_TAG_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

class TagTable
{
public:
	TagTable(SqlConnection &sql);
	virtual ~TagTable();
private:
	TagTable();

public:
	void check(QStringList &tables);

	void insert(const Tag &t);
	void update(const Tag &orig, const Tag &modified);
	void del(const Tag &t);
	void query(TagSet &tags);

private:
	SqlConnection &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

#endif
