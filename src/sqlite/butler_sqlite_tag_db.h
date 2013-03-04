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
	bool create();
	bool check(QStringList &tables);
	bool update();

	bool insert(const Tag &t);
	bool update(const Tag &orig, const Tag &modified);
	bool del(const Tag &t);
	bool query(TagSet &ts);

private:
	Sql &sql;
	TagTable tagTable;
};

}

#endif

