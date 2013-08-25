/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_TABLE_H
#define BUTLER_TAG_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_sql_connection.h>

#include <butler_tag_set.h>

class TagTable
{
public:
	TagTable(SqlConnection &sql);
	virtual ~TagTable();
private:
	TagTable();

public:
	void check(QStringList &tables);

	void insert(const Tag &t);
	void update(const Tag &orig, const Tag &modified);
	void del(const Tag &t);
	void query(TagSet &tags);

private:
	SqlConnection &sql;
};

#endif
