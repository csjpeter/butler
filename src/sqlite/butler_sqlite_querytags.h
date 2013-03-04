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
	bool create();
	bool check(QStringList &tables);

	bool insert(const Query &q, const QString &tagName);
	bool del(const Query &q, const QString &tagName);

	bool insert(const Query &q);
	bool update(const Query &orig, const Query &modified);
	bool del(const Query &q);
	bool query(const Query &q, TagNameSet &tags);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif


