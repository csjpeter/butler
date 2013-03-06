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
	void check(QStringList &tables);
	void insert(const Ware &, const QString &tag);
	void del(const Ware &, const QString &tag);
	void insert(const Ware &);
	void update(const Ware &orig,
			const Ware &modified);
	void query(const Ware &, csjp::OwnerContainer<QString> &tags);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
};

}

#endif
