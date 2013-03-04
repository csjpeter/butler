/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_WARECATEGORIES_H
#define BUTLER_SQLITE_WARECATEGORIES_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

#include "butler_sqlite_ware.h"

namespace Sqlite {

class WareCategoriesTable
{
public:
	WareCategoriesTable(Sql &_sql);
	virtual ~WareCategoriesTable();
private:
	WareCategoriesTable();

public:
	bool create();
	bool check(QStringList &tables);
	bool insert(const Ware &, const QString &category);
	bool del(const Ware &, const QString &category);
	bool insert(const Ware &);
	bool update(const Ware &orig,
			const Ware &modified);
	bool query(const Ware &, CategoryNameSet &categories);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif
