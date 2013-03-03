/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_WARETAGS_H
#define BUTLER_SQLITE_WARETAGS_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

#include "butler_sqlite_tag.h"

namespace Sqlite {

class WareTagsTable
{
public:
	WareTagsTable(Sql &_sql);
	virtual ~WareTagsTable();
private:
	WareTagsTable();

public:
	bool create();
	bool check(QStringList &tables);
	bool insert(const Ware &, const QString &tag);
	bool del(const Ware &, const QString &tag);
	bool insert(const Ware &);
	bool update(const Ware &orig,
			const Ware &modified);
	bool query(const Ware &, csjp::OwnerContainer<QString> &tags);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif
