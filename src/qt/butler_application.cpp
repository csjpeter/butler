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

#include "butler_application.h"

/*using namespace std;*/

namespace Butler {

Application::Application(int &argc, char *argv[]) :
	QApplication(argc, argv)
{
	QCoreApplication::setOrganizationName(STRING(AUTHOR));
	QCoreApplication::setOrganizationDomain("no.domain");
	QCoreApplication::setApplicationName("ButlerQt");
}

Application::~Application()
{
}

bool Application::notify(QObject * receiver, QEvent * event)
{
	QString info;

	try {
		return QApplication::notify(receiver, event);
	} catch(std::exception& e) {
		info = e.what();
	}

	QMessageBox(	QMessageBox::Warning, "Exception thrown", info,
			QMessageBox::Ok, 0, Qt::Dialog).exec();

	return false; /* Was not handled. */
}

}

