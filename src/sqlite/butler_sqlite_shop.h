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
	void check(QStringList &tables);
	void insert(const Shop &);
	void update(const Shop &orig,
			const Shop &modified);
	void del(const Shop &);
	void query(ShopSet &);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif


