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

#ifdef DEBUG
	csjp::verboseMode = true;
#endif
	csjp::verboseMode = true;

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

		if(!strcmp(args[argi], "")) /* The rests are Qt arguments. */
			break;

		fprintf(stderr, "Bad argument given: '%s'\n", args[argi]);
		DBG("Bad argument given: '%s'", args[argi]);
		return 1;
	}

	try {
		Path::initRootPath(args[0]);

		Config::loadTranslation();
		Config::loadTranslation("hu");

		/* Init the local database */
		if(dbFileName == ""){
			QSettings settings;
			dbFileName = settings.value("mainview/dbfile", QString()).toString();
		}
		if(dbFileName == ""){
			QDir dir(QDir::homePath());

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


		/* Init the styleshhet */
		QString cssFileName(Path::css("application.css"));
		QFile cssFile(cssFileName);
		if(!cssFile.open(QIODevice::ReadOnly))
			throw csjp::FileError("Cant open css file: %s", C_STR(cssFileName));
		QByteArray data = cssFile.readAll();
		app.setStyleSheet(data);
		//DBG("CSS content:\n%s", data.constData());

		/* Mainview */
		MainView view(defaultDbName);
		view.show();

		/* Run the app */
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
