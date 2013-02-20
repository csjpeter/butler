/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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
			CRITICAL("Connect to database engine failed");
			CRITICAL(qPrintable(error()));
		}

		QFile f(filepath);
		if(!f.exists()){
			if(!_db->create()){
				CRITICAL("Create database failed");
				CRITICAL(qPrintable(error()));
			}
		}

		if(!_db->open()){
			CRITICAL("Open database failed");
			CRITICAL(qPrintable(error()));
		}

		if(!_db->check()){
			CRITICAL("Check database schema validity failed");
			CRITICAL(qPrintable(error()));
		}
	}

	refCount++;
}

LocalDb::~LocalDb()
{
	refCount--;
	if(!refCount){
		if(!_db->close()){
			CRITICAL("Close database failed");
			CRITICAL(qPrintable(error()));
		}
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
	msg += db().lastUserError();
	msg += QString("\n");
	msg += QString("Database error: ");
	msg += QString("\n");
	msg += db().lastError();

	return msg;
}

}



