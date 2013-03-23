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
#include "butler_config.h"

int main(int argc, char *args[])
{
	QString dbFileName;
	QString defaultDbName("localdb");

	csjp::setBinaryName(args[0]);
	Application app(argc, args);
	app.addLibraryPath("/data/data/org.kde.necessitas.ministro/files/qt/plugins"); 

	DBG("QCoreApplication::applicationDirPath(): %s", C_STR(app.applicationDirPath()));
	DBG("QDir::homePath(): %s", C_STR(QDir::homePath()));

	int argi = 1;

	if(1 <= argc - argi && (
			  !strcmp(args[argi], "--help") ||
			  !strcmp(args[argi], "-h"))){
		printf(	"Usage: %s [--dbfile|-d file] [--logdir|-l dir] [--verbose|-v]\n"
			"       %s [--help|-h]\n"
			"\n", args[0], args[0]);
		return 0;
	}

	while(argi < argc){
		if(2 <= argc - argi && (
				  !strcmp(args[argi], "--dbfile") ||
				  !strcmp(args[argi], "-d"))){
			dbFileName = QString(args[argi+1]);
			printf("Using %s as database file.\n", args[argi+1]);
			argi += 2;
			continue;
		}
		if(2 <= argc - argi && (
				  !strcmp(args[argi], "--logdir") ||
				  !strcmp(args[argi], "-l"))){
			csjp::setLogDir(args[argi+1]);
			argi += 2;
			continue;
		}
		if(1 <= argc - argi && (
				  !strcmp(args[argi], "--verbose") ||
				  !strcmp(args[argi], "-v"))){
			csjp::verboseMode = true;
			argi++;
			continue;
		}
		fprintf(stderr, "Bad arguments given.\n");
		return 1;
	}

	try {
		Path::initRootPath(args[0]);
		DBG("Path::icon(""): %s", C_STR(Path::icon("")));
	} catch (csjp::Exception & e) {
		EXCEPTION(e);
		return -1;
	} catch (std::exception & e) {
		fprintf(stderr, "Failed to determine the installation root directory. "
				"Unexpected exception: %s", e.what());
		return -1;
	}

	try {
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

		DBG("dbFileName %s", C_STR(dbFileName));
		csjp::Object<DatabaseDescriptor> sqlitedb(new DatabaseDescriptor);
		sqlitedb->name = defaultDbName;
		sqlitedb->driver = "QSQLITE";
		sqlitedb->databaseName = QDir::toNativeSeparators(dbFileName);
		DBG("Db file path: %s", C_STR(sqlitedb->databaseName));
		registerDatabase(sqlitedb);
	} catch (csjp::Exception & e) {
		EXCEPTION(e);
		return -1;
	} catch (std::exception & e) {
		fprintf(stderr, "Failed to initialize database. Unexpected exception: %s",
				e.what());
		return -1;
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

	try {
		MainView view(defaultDbName);
		//view.setFixedSize(800, 480);
		view.show();

		return app.exec();
	} catch (csjp::Exception & e) {
		EXCEPTION(e);
		return -1;
	} catch (std::exception & e) {
		fprintf(stderr, "Failed main view construction or other unexpected exception: %s",
				e.what());
		return -1;
	}
}
