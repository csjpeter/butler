/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SHOP_H
#define BUTLER_SQLITE_SHOP_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

namespace Sqlite {

class ShopTable
{
public:
	ShopTable(Sql &sql);
	virtual ~ShopTable();
private:
	ShopTable();

public:
	bool create();
	bool check(QStringList &tables);
	bool insert(const Shop &);
	bool update(const Shop &orig,
			const Shop &modified);
	bool del(const Shop &);
	bool query(ShopSet &);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif


