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

namespace Sqlite {

class TagTable
{
public:
	TagTable(Sql &sql);
	virtual ~TagTable();
private:
	TagTable();

public:
	bool create();
	bool check(QStringList &tables);
	bool alter();

	bool insert(const Tag &t);
	bool update(const Tag &orig, const Tag &modified);
	bool del(const Tag &t);
	bool query(TagSet &tags);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif

