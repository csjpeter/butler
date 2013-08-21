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
	csjp::setBinaryName(args[0]);
	Application app(argc, args);

#ifdef DEBUG
	csjp::verboseMode = true;
#endif

	int argi = 1;

	if(1 <= argc - argi && (
			  !strcmp(args[argi], "--help") ||
			  !strcmp(args[argi], "-h"))){
		printf(	"Usage: %s [--logdir|-l dir] [--verbose|-v]\n"
			"       %s [--help|-h]\n"
			"\n", args[0], args[0]);
		return 0;
	}

	while(argi < argc){
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
		LOG("Bad argument given: '%s'", args[argi]);
		return 1;
	}

#ifdef Q_OS_ANDROID
	QDir filesDir("/data/data/com." PRJNAME "/files");
	filesDir.mkpath("qt/plugins/platform/android");
	filesDir.mkpath("qt/plugins/sqldrivers");
/*
	const char * libandroid = "/data/data/com." PRJNAME "/files/"
			"qt/plugins/platform/android/libandroid-9.so";
	if(!QFile::remove(libandroid))
		LOG("Failed to remove %s", libandroid);
	if(!QFile::copy("/data/data/com." PRJNAME "/lib/libandroid-9.so", libandroid))
		LOG("Failed to create %s", libandroid);
*/
	const char * libqsqlpsql = "/data/data/com." PRJNAME "/files/"
			"qt/plugins/sqldrivers/libqsqlpsql.so";
	if(!QFile::remove(libqsqlpsql))
		LOG("Failed to remove %s", libqsqlpsql);
	if(!QFile::copy("/data/data/com." PRJNAME "/lib/libqsqlpsql.so", libqsqlpsql))
		LOG("Failed to create %s", libqsqlpsql);

	app.addLibraryPath("/data/data/org.kde.necessitas.ministro/files/qt/plugins");
	app.addLibraryPath("/data/data/org.kde.necessitas.ministro/files/dl/0/stable//plugins");
	app.addLibraryPath("/data/data/com." PRJNAME "/files/qt/plugins");
#endif

	try {
		LOG("Environment variables:\n%s",
			C_STR(QProcessEnvironment::systemEnvironment().toStringList().join("\n")));

		Path::initRootPath(args[0]);
		Path::initConfigFileName();
		app.setWindowIcon(QIcon(Path::icon("butler.png")));

		app.loadTranslation();
		app.loadTranslation("hu");
		app.pixelPerMM();
		Config::thresholdScrollDistance = Config::pxPerMM * 2;
		DBG("kinetic scroll treshold: %d", Config::thresholdScrollDistance);
		app.loadCSS();
		Config::load();
		loadDatabaseConfigs();

		LOG("Library paths: %s", C_STR(qApp->libraryPaths().join(", ")));

		/* Show main view and run the app */
		app.mainView().show();
		bool ret = app.exec();
		saveDatabaseConfigs();
		Config::save();
		return ret;
	} catch (csjp::Exception & e) {
		EXCEPTION(e);
		return -1;
	} catch (std::exception & e) {
		fprintf(stderr, "Failed main view construction or other unexpected exception: %s",
				e.what());
		return -1;
	}
}
