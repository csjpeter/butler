/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARECATEGORIES_TABLE_H
#define BUTLER_WARECATEGORIES_TABLE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class WareCategoriesTable
{
public:
	WareCategoriesTable(SqlConnection &_sql);
	virtual ~WareCategoriesTable();
private:
	WareCategoriesTable();

public:
	void check(QStringList &tables);
	void insert(const Ware &, const QString &category);
	void del(const Ware &, const QString &category);
	void insert(const Ware &);
	void update(const Ware &orig,
			const Ware &modified);
	void query(const Ware &, CategoryNameSet &categories);

private:
	SqlConnection &sql;
};

#endif
