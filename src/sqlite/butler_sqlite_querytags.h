/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_QUERYTAGS_H
#define BUTLER_SQLITE_QUERYTAGS_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

#include "butler_sqlite_tag.h"

namespace Sqlite {

class QueryTagsTable
{
public:
	QueryTagsTable(Sql &_sql);
	virtual ~QueryTagsTable();
private:
	QueryTagsTable();

public:
	void check(QStringList &tables);

	void insert(const Query &q, const QString &tagName);
	void del(const Query &q, const QString &tagName);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(const Query &q, TagNameSet &tags);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif


