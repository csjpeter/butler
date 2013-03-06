/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

/*#include <iostream>*/

#include <QtGui>

#include <config.h>

#include <butler_sqlitedb.h>

#include "butler_application.h"

Application::Application(int &argc, char *argv[]) :
	QApplication(argc, argv)
{
	QCoreApplication::setOrganizationName(AUTHOR);
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
	} catch(csjp::Exception& e) {
		EXCEPTION(e);
		info = e.what();
	} catch(std::exception& e) {
		info = e.what();
	}

	QMessageBox(	QMessageBox::Warning, "Exception thrown", info,
			QMessageBox::Ok, 0, Qt::Dialog).exec();

	return false; /* Was not handled. */
}
