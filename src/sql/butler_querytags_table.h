/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYTAGS_TABLE_H
#define BUTLER_QUERYTAGS_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_sql_connection.h>

#include <butler_query_set.h>

class QueryTagsTable
{
public:
	QueryTagsTable(SqlConnection &_sql);
	virtual ~QueryTagsTable();
private:
	QueryTagsTable();

public:
	void check(QStringList &tables);

	void insert(const Query &q, const QString &tagName);
	void del(const Query &q, const QString &tagName);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(const Query &q, TagNameSet &tags);

private:
	SqlConnection &sql;
};

#endif
