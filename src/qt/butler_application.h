/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_APPLICATION_H
#define BUTLER_APPLICATION_H

#include <QApplication>

/* Forwards: */
class QSessionManager;

namespace Butler {

class Application : public QApplication
{
public:
	Application(int &argc, char *argv[]);
	virtual ~Application();

	virtual bool notify(QObject * receiver, QEvent * event);
};

}

#endif

