/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SCHEMAVERSION_DB_H
#define BUTLER_SQLITE_SCHEMAVERSION_DB_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

#define BUTLER_SQLITE_SCHEMA_VERSION_MAJOR 0
#define BUTLER_SQLITE_SCHEMA_VERSION_MINOR 1
#define BUTLER_SQLITE_SCHEMA_VERSION_PATCH 1

namespace Sqlite {

struct Version {
	int major;
	int minor;
	int patch;
};

class SchemaVersion
{
public:
	SchemaVersion(Sql &sql);
	virtual ~SchemaVersion();
private:
	SchemaVersion();

public:
	bool create();
	bool check(QStringList &tables);
	bool update();
	bool update(const Version &version);
	bool query(Version &version);

private:
	Sql &sql;
};

}

#endif

