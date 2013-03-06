/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_APPLICATION_H
#define BUTLER_APPLICATION_H

#include <QApplication>

/* Forwards: */
class QSessionManager;

class Application : public QApplication
{
public:
	Application(int &argc, char *argv[]);
	virtual ~Application();

	virtual bool notify(QObject * receiver, QEvent * event);
};

#endif
