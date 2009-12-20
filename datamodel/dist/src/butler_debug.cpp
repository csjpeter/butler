/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <QDebug>
#include <QString>
#include <QTime>
#include <QStack>
#include <QMultiMap>

#include "butler_debug.h"

#ifndef NO_RUNTIME_BACKTRACE
#ifndef COMPONENT_NAME
#error Please define COMPONENT_NAME on compiler command line to be used as prefix for function call debug messages.
#endif
#ifndef COMPONENT_COLOR
#error Please define COMPONENT_COLOR (ASCII terminal colors, for help look at butler_debug.h ) on compiler command line to be used as color for COMPONENT_NAME and function call depth markers.
#endif

static QTime timer;
static bool started = false;
static QString inPrefix, outPrefix;
static QStack<int> timeStack;
static QMap<QString, bool> leakCollection;

/* big hack :( */
void _nothing(){}

/* Exception might happen while constructing, so only at leave we can sure a
 * the construction realy happened. */
void _runtimeBacktraceLeaveConstructor(void *ptr, const char *funcName)
{
	QString str = funcName;
	str = str.section("::", 0, 0);
	str << " " << ptr;
	if(!leakCollection.contains(str))
		leakCollection.insert(str, true);
	else {
		if(leakCollection[str])
			qCritical("Reallocating %s on %p for which no "
					"destructor run yet.");
		leakCollection[str] = true;
	}
}

void _runtimeBacktraceLeaveDestructor(void *ptr, const char *funcName)
{
	QString str = funcName;
	str = str.section("::", 0, 0);
	str << " " << ptr;
	leakCollection[str] = false;
}

void _reportLeakSuspections(){
	qDebug("List of not yet destructed objects:");
	QMap<QString, bool>::Iterator iter;
	for(iter=leakCollection.begin(); iter != leakCollection.end(); iter++){
		if(iter.value())
			qDebug(iter.key());
	}
}

void _runtimeBacktraceEnter(void *ptr, const char *funcName, int verbose)
{
	if(!started){
		timer.start();
		started = true;
	}

	timeStack.push(timer.elapsed());
	inPrefix.append("> ");
	outPrefix.append("< ");
	fprintf(stderr, "%s%16s%s %12p %s%12s %s%s%s %s",
			COMPONENT_COLOR, COMPONENT_NAME, VT_TA_ALLOFF,
			ptr,
			VT_FG_BLUE, qPrintable(
			  QTime::currentTime().toString("HH:mm:ss.zzz")),
			COMPONENT_COLOR, qPrintable(inPrefix), VT_TA_ALLOFF,
			funcName);
	if(verbose != __INT_MAX__)
		fprintf(stderr, " %sv%d%s",
				VT_FG_BLUE, verbose, VT_TA_ALLOFF);
	fprintf(stderr, "\n");
}

void _runtimeBacktraceLeave(void *ptr, const char *funcName, int verbose)
{
	int pop = timeStack.isEmpty() ? 0 : timeStack.pop();
	qDebug("%s%s%s %12p %s%11.3fs %s%s%s %s",
			ptr,
			VT_FG_MAGENTA, (timer.elapsed()-pop) / 1000.0,
			VT_FG_CYAN, qPrintable(outPrefix), VT_TA_ALLOFF,
			funcName);
	fprintf(stderr, "%s%16s%s %12p %s%11.3fs %s%s%s %s",
			COMPONENT_COLOR, COMPONENT_NAME, VT_TA_ALLOFF,
			ptr,
			VT_FG_MAGENTA, ((timer.elapsed()-pop) / 1000.0),
			COMPONENT_COLOR, qPrintable(outPrefix), VT_TA_ALLOFF,
			funcName);
	if(verbose != __INT_MAX__)
		fprintf(stderr, " %sv%d%s",
				VT_FG_BLUE, verbose, VT_TA_ALLOFF);
	fprintf(stderr, "\n");
	inPrefix.chop(2);
	outPrefix.chop(2);
}

#endif

