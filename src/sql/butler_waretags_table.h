/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARETAGS_TABLE_H
#define BUTLER_WARETAGS_TABLE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class WareTagsTable
{
public:
	WareTagsTable(SqlConnection &_sql);
	virtual ~WareTagsTable();
private:
	WareTagsTable();

public:
	void check(QStringList &tables);
	void insert(const Ware &, const QString &tag);
	void del(const Ware &, const QString &tag);
	void insert(const Ware &);
	void update(const Ware &orig,
			const Ware &modified);
	void query(const Ware &, csjp::OwnerContainer<QString> &tags);

private:
	SqlConnection &sql;
};

#endif
