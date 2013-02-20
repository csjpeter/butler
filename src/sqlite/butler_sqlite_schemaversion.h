/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

