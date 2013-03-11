/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_object.h>

#include <QtGui>

#include "butler_application.h"
#include "butler_databases.h"
#include "butler_mainview.h"

int main(int argc, char *argv[])
{
/*	QT_REQUIRE_VERSION(argc, argv, "4.0.2");*/
#ifdef DEBUG
	csjp::setLogDir("./");
#endif
	QString dbFileName;
	QString defaultDbName("localdb");

	Application app(argc, argv);

	if(1 < argc){
		if(QString(argv[1]) == "--help"){
			printf("Usage: %s [--dbfile file]\n\n", argv[0]);
		} else if(QString(argv[1]) == "--dbfile"){
			if(argc < 2){
				printf("There should be a filename as second parameter.\n");
				exit(EXIT_FAILURE);
			}
			printf("Using %s as database file.\n", argv[2]);
			dbFileName = QString(argv[2]);
		}
	}

	{
		if(dbFileName == ""){
			QSettings settings;
			dbFileName = settings.value("mainview/dbfile", QString()).toString();
		}
		
		if(dbFileName == ""){
			QDir dir(QDir::homePath());

			/* TODO check if .butler is directory and not file */
			if(!dir.exists(".butler"))
				dir.mkdir(".butler");
			dbFileName = QDir::toNativeSeparators(
				QDir::homePath() + QString("/.butler/db.sqlite")
				);
		}
		
		csjp::Object<DatabaseDescriptor> sqlitedb(new DatabaseDescriptor);
		sqlitedb->name = defaultDbName;
		sqlitedb->driver = "QSQLITE";
		sqlitedb->databaseName = QDir::toNativeSeparators(dbFileName);
		DBG("Db file path: %s", C_STR(sqlitedb->databaseName));
		registerDatabase(sqlitedb);
	}

#ifdef MAEMO
#define CSS_BUFFER_SIZE 64*1024
	{
		MSG("Loading css file...");
		char buf[CSS_BUFFER_SIZE];
		QFile cssFile("/usr/share/butler/css/maemo.css");
		if(cssFile.open(QIODevice::ReadOnly)){
			int read = cssFile.read(buf, CSS_BUFFER_SIZE-1);
			if(0 < read){
				buf[read] = 0;
				MSG("%s", buf);
				app.setStyleSheet(buf);
			}
		} else {
			WARNING("Cant open css file");
		}
	}
#endif

	MainView view(defaultDbName);
//	view.setFixedSize(800, 480);
	view.show();

	return app.exec();
}
