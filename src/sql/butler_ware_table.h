/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_TABLE_H
#define BUTLER_WARE_TABLE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class WareTable
{
public:
	WareTable(SqlConnection &sql);
	virtual ~WareTable();
private:
	WareTable();

public:
	void check(QStringList &tables);
	void insert(const Ware &);
	void update(const Ware &orig,
			const Ware &modified);
	void del(const Ware &);
	void query(WareSet &);

private:
	SqlConnection &sql;
};

#endif
