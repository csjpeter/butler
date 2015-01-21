/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MACROS_H
#define BUTLER_MACROS_H

#include <QCoreApplication>

#define C_STR(QStr) QStr.toUtf8().constData()

#define NO_FCLOG __attribute__ ((no_instrument_function))

#define SAVE_VIEW_STATE(view) \
	if(view) { \
		view->saveState(); \
		if(view->isVisible()) \
			settings.setValue(prefix + "/" #view, true); \
		else \
			settings.setValue(prefix + "/" #view, false); \
	}

#define MY_Q_OBJECT \
	QString trShort(const char * sourceText, const char * context = 0, \
			const char * disambiguation = 0, int n = -1) { \
		return translationVariant(QCoreApplication::translate( \
				  context ? context : this->metaObject()->className(), \
				  sourceText, disambiguation, n ), TextVariant::Short); \
	} \
	QString trMed(const char * sourceText, const char * context = 0, \
			const char * disambiguation = 0, int n = -1) { \
		return translationVariant(QCoreApplication::translate( \
				  context ? context : this->metaObject()->className(), \
				  sourceText, disambiguation, n ), TextVariant::Medium); \
	} \
	QString trLong(const char * sourceText, const char * context = 0, \
			const char * disambiguation = 0, int n = -1) \
	{ \
		return translationVariant(QCoreApplication::translate( \
				  context ? context : this->metaObject()->className(), \
				  sourceText, disambiguation, n ), TextVariant::Long); \
	}

#define SCC static const char*

#endif
