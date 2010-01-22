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

#include <ButlerDebug>
#include "butler_sqlite_schemaversion.h"

namespace Butler {
namespace Sqlite {

	SchemaVersionDb::SchemaVersionDb(Db &_db) :
		db(_db)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	SchemaVersionDb::~SchemaVersionDb()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	bool SchemaVersionDb::create()
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery query(db.db);
		query.exec("CREATE TABLE SchemaVersion ("
				"major INTEGER, "
				"minor INTEGER, "
				"patch INTEGER"
				")"
				);
		ret = db.reportSqlError();
		
		if(ret){
			QSqlQuery sqlQuery(db.db);
			QString query;
			query.sprintf("INSERT INTO SchemaVersion "
				"(major, minor, patch) "
				"VALUES(%d, %d, %d)",
				BUTLER_SQLITE_SCHEMA_VERSION_MAJOR,
				BUTLER_SQLITE_SCHEMA_VERSION_MINOR,
				BUTLER_SQLITE_SCHEMA_VERSION_PATCH);
			sqlQuery.exec(query);
			ret = db.reportSqlError();
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool SchemaVersionDb::check(QStringList &tables)
	{
		ENTER_FUNCTION();
		bool ret = true;

		ret = tables.contains("SchemaVersion");

		if(ret){
			QSqlRecord table = db.db.record("SchemaVersion");
			if(		!table.contains("major") ||
					!table.contains("minor") ||
					!table.contains("patch")
			  ) {
				ret = false;
				qCritical("Incompatible table SchemaVersion "
						"in the openend database.");
			}
		}

		LEAVE_FUNCTION();
		return ret;
	}

	bool SchemaVersionDb::update()
	{
		return true;
	}

	bool SchemaVersionDb::updateSchemaVersion(const SchemaVersion &version)
	{
		ENTER_FUNCTION();
		bool ret;

		QSqlQuery sqlQuery(db.db);
		QString query;
		query = "UPDATE SchemaVersion SET major = '";
		query += version.major;
		query += "', minor = ' ";
		query += version.minor;
		query += "', patch = ' ";
		query += version.patch;
		query += "' WHERE ROWID = 0";
		sqlQuery.exec(query);
		ret = db.reportSqlError();

		LEAVE_FUNCTION();
		return ret;
	}

	SchemaVersion SchemaVersionDb::querySchemaVersion()
	{
		ENTER_FUNCTION();

		QSqlQuery query(db.db);
		query.setForwardOnly(true);
		query.prepare("SELECT major, minor, patch FROM SchemaVersion");

		query.exec();
		db.reportSqlError();

		SchemaVersion version;
		version.major = 0;
		version.minor = 0;
		version.patch = 0;

		v1Debug("----- Version checking:");
		if(query.next()) {
			version.major = query.value(0).toInt();
			version.minor = query.value(1).toInt();
			version.patch = query.value(2).toInt();
			v1Debug("SchemaVersion: %d.%d.%d",
					version.major,
					version.minor,
					version.patch);
		}
		v1Debug("-----");

		query.finish();

		LEAVE_FUNCTION();
		return version;
	}
}
}

