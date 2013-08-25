/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_TABLE_H
#define BUTLER_ITEM_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_sql_connection.h>

#include <butler_item_set.h>

class ItemTable
{
public:
	ItemTable(SqlConnection &sql);
	virtual ~ItemTable();
private:
	ItemTable();

public:
	void check(QStringList &tables);
	void insert(const Item &);
	void update(const Item &orig,
			const Item &modified);
	void del(const Item &);
	void query(Item &);
	/* results only with unbought items */
	void query(const TagNameSet &, ItemSet &);

private:
	SqlConnection &sql;
};

#endif
