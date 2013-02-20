/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <csjp_debug.h>

#include "butler_sqlite_schemaversion.h"

namespace Sqlite {

SchemaVersion::SchemaVersion(Sql &_sql) :
	sql(_sql)
{
}

SchemaVersion::~SchemaVersion()
{
}

bool SchemaVersion::create()
{
	bool ret;

	ret = sql.exec("CREATE TABLE SchemaVersion ("
			"major INTEGER, "
			"minor INTEGER, "
			"patch INTEGER"
			")"
			);
	
	if(ret){
		QString cmd;
		cmd.sprintf("INSERT INTO SchemaVersion "
			"(major, minor, patch) "
			"VALUES(%d, %d, %d)",
			BUTLER_SQLITE_SCHEMA_VERSION_MAJOR,
			BUTLER_SQLITE_SCHEMA_VERSION_MINOR,
			BUTLER_SQLITE_SCHEMA_VERSION_PATCH);
		ret = sql.exec(cmd);
	}

	return ret;
}

bool SchemaVersion::check(QStringList &tables)
{
	bool ret = true;

	ret = tables.contains("SchemaVersion");

	if(ret){
		QSqlRecord table = sql.record("SchemaVersion");
		if(		!table.contains("major") ||
				!table.contains("minor") ||
				!table.contains("patch")
		  ) {
			ret = false;
			CRITICAL("Incompatible table SchemaVersion "
					"in the openend database.");
		}
	}

	return ret;
}

bool SchemaVersion::update()
{
	return true;
}

bool SchemaVersion::update(const Version &version)
{
	bool ret;

	QString cmd;
	cmd.sprintf(	"UPDATE SchemaVersion SET "
			"major = '%d', minor = '%d', patch = '%d' "
			"WHERE ROWID = 0",
			version.major,
			version.minor,
			version.patch);
	ret = sql.exec(cmd);

	return ret;
}

bool SchemaVersion::query(Version &version)
{
	bool ret;
	SqlQuery sqlQuery(sql);

	ret = sqlQuery.exec(
			"SELECT major, minor, patch FROM SchemaVersion"
			);

	version.major = 0;
	version.minor = 0;
	version.patch = 0;

	DBG("----- Version checking:");
	if(sqlQuery.next()) {
		version.major = sqlQuery.value(0).toInt();
		version.minor = sqlQuery.value(1).toInt();
		version.patch = sqlQuery.value(2).toInt();
		DBG("Found database schema version: %d.%d.%d",
				version.major,
				version.minor,
				version.patch);
	}
	DBG("-----");
	DBG("Software backend provides database schema version %d.%d.%d",
			BUTLER_SQLITE_SCHEMA_VERSION_MAJOR,
			BUTLER_SQLITE_SCHEMA_VERSION_MINOR,
			BUTLER_SQLITE_SCHEMA_VERSION_PATCH);

	sqlQuery.finish();

	return ret;
}

}

