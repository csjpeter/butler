/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <syslog.h>
#include <stdio.h>

#include <QtGui>

#include "butler_application.h"
#include "butler_localdb.h"
#include "butler_mainview.h"

int main(int argc, char *argv[])
{
/*	QT_REQUIRE_VERSION(argc, argv, "4.0.2");*/

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

