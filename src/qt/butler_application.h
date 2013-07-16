/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_APPLICATION_H
#define BUTLER_APPLICATION_H

#include <QApplication>

#include "butler_mainview.h"

/* Forwards: */
class QSessionManager;

class Application : public QApplication
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	Application(int &argc, char *argv[]);
	virtual ~Application();

	void loadTranslation(const char * langCode = NULL);
	void pixelPerMM();
	void loadCSS();

	MainView & mainView();

	virtual bool notify(QObject * receiver, QEvent * event);

private:
	MainView * mainViewPtr;
};

#endif
