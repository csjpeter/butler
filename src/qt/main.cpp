/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <QtGui>

#include "butler_application.h"
#include "butler_localdb.h"
#include "butler_mainview.h"

int main(int argc, char *argv[])
{
/*	QT_REQUIRE_VERSION(argc, argv, "4.0.2");*/

	csjp::setLogDir("./");

	Butler::Application app(argc, argv);

	if(1 < argc){
		if(QString(argv[1]) == "--help"){
			printf("Usage: %s [--dbfile file]\n\n", argv[0]);
		} else if(QString(argv[1]) == "--dbfile"){
			if(argc < 2){
				printf("There should be a filename as second parameter.\n");
				exit(EXIT_FAILURE);
			}
			printf("Using %s as database file.\n", argv[2]);
			Butler::LocalDb::dbFileName = QString(argv[2]);
		}
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

	Butler::MainView &view = Butler::MainView::instance();
//	view.setFixedSize(800, 480);
	view.show();

	return app.exec();
}

