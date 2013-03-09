/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_DB_H
#define BUTLER_TAG_DB_H

#include <butler_tag_table.h>

class TagDb
{
public:
	TagDb(SqlConnection &sql);
	virtual ~TagDb();
private:
	TagDb();

public:
	void check(QStringList &tables);

	void insert(const Tag &t);
	void update(const Tag &orig, const Tag &modified);
	void del(const Tag &t);
	void query(TagSet &ts);

private:
	SqlConnection &sql;
	TagTable tagTable;
};

#endif
