/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEMBOUGHT_TABLE_H
#define BUTLER_ITEMBOUGHT_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_sql_connection.h>

#include <butler_item_set.h>
#include <butler_query_set.h>

class ItemBoughtTable
{
public:
	ItemBoughtTable(SqlConnection &sql);
	virtual ~ItemBoughtTable();
private:
	ItemBoughtTable();

public:
	void check(QStringList &tables);
	void insert(const Item &i);
	void update(const Item &orig,
			const Item &modified);
	void del(const Item &item);
	void query(const Query &q, QueryStat &stat, ItemSet &items);

private:
	SqlConnection &sql;
};

#endif
