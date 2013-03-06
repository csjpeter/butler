/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_TAG_DB_H
#define BUTLER_SQLITE_TAG_DB_H

#include <butler_db.h>

#include "butler_sqlite_tag.h"

namespace Sqlite {

class TagDb : public ::TagDb
{
public:
	TagDb(Sql &sql);
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
	Sql &sql;
	TagTable tagTable;
};

}

#endif

