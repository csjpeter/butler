/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

/*#include <iostream>*/

#include <QtGui>

#include <butler_sqlitedb.h>

#include "butler_localdb.h"

namespace Butler {

int LocalDb::refCount = 0;
Db* LocalDb::_db = NULL;
QString LocalDb::dbFileName = "";
	
LocalDb::LocalDb()
{
	if(_db == NULL){
		QString filepath;

		if(dbFileName == ""){
			QSettings settings;
			Butler::LocalDb::dbFileName = settings.value(
					"mainview/dbfile", QString()).toString();
		}
		
		if(dbFileName == ""){
			filepath = QDir::toNativeSeparators(
				QDir::homePath() + QString("/.butler/db.sqlite")
				);
		} else {
			filepath = QDir::toNativeSeparators(dbFileName);
		}

		QDir dir(QDir::homePath());

		/* TODO check if .butler is directory and not file */
		if(!dir.exists(".butler"))
			dir.mkdir(".butler");

		DBG("Db file path: %s", qPrintable(filepath));
		_db = new SqliteDb(filepath);

		if(!_db->connect()){
			QFile f(filepath);
			if(!f.exists())
				if(!_db->create())
					throw DbError("Create database failed. "
							"ErrorId: %u, Error message: %s",
							_db->lastErrorId(),
							C_STR(_db->lastError()));
			if(!_db->connect())
				throw DbError("Connect to database engine failed. "
						"ErrorId: %u, Error message: %s",
						_db->lastErrorId(),
						C_STR(_db->lastError()));
		}

		if(!_db->open())
			throw DbError("Open database failed. "
					"ErrorId: %u, Error message: %s",
					_db->lastErrorId(),
					C_STR(_db->lastError()));

		if(!_db->check())
			throw DbError("Database schema validity check failed. "
					"ErrorId: %u, Error message: %s",
					_db->lastErrorId(),
					C_STR(_db->lastError()));
	}

	refCount++;
}

LocalDb::~LocalDb()
{
	refCount--;
	if(!refCount){
		if(!_db->close())
			csjp::msgLogger(stderr, "", VT_RED VT_TA_BOLD "Close database failed" VT_NORMAL);
		delete _db;
		_db = NULL;
	}
}

Db& LocalDb::db()
{
	return *_db;
}

QString LocalDb::error(QString err)
{
	QString msg;
	if(err.size()){
		msg += err;
		msg +=QString("\n");
	}

	msg += QString("Reported error message: ");
	msg += QString("\n");
	msg += QString("Database error: ");
	msg += QString("\n");
	msg += db().lastError();

	return msg;
}

}



