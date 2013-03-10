/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOP_TABLE_H
#define BUTLER_SHOP_TABLE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class ShopTable
{
public:
	ShopTable(SqlConnection &sql);
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
	SqlConnection &sql;
};

#endif
