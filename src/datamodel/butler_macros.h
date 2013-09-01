/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MACROS_H
#define BUTLER_MACROS_H

#include <QCoreApplication>
#include <QVariant>
#include <csjp_string.h>
#include <csjp_text.h>
#include <csjp_datetime.h>

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
				  sourceText, disambiguation, QCoreApplication::UnicodeUTF8, n \
				  ), TextVariant::Short); \
	} \
	QString trMed(const char * sourceText, const char * context = 0, \
			const char * disambiguation = 0, int n = -1) { \
		return translationVariant(QCoreApplication::translate( \
				  context ? context : this->metaObject()->className(), \
				  sourceText, disambiguation, QCoreApplication::UnicodeUTF8, n \
				  ), TextVariant::Medium); \
	} \
	QString trLong(const char * sourceText, const char * context = 0, \
			const char * disambiguation = 0, int n = -1) \
	{ \
		return translationVariant(QCoreApplication::translate( \
				  context ? context : this->metaObject()->className(), \
				  sourceText, disambiguation, QCoreApplication::UnicodeUTF8, n \
				  ), TextVariant::Long); \
	}

#define SCC static const char*

inline csjp::String & operator<<= (csjp::String & str, const QString & qstr)
{
	str = C_STR(qstr); return str;
}

inline QString & operator<<= (QString & qstr, const csjp::String & str)
{
	qstr = str.c_str(); return qstr;
}

inline csjp::Text & operator<<= (csjp::Text & text, const QString & s)
{
	text = s.utf16(); return text;
}

inline csjp::Text & operator<<= (csjp::Text & text, const QVariant & v)
{
	text = v.toString().utf16(); return text;
}

inline QString & operator<<= (QString & qstr, const QVariant & v)
{
	qstr = v.toString(); return qstr;
}

inline int & operator<<= (int & i, const QVariant & v)
{
	i = v.toInt(); return i;
}

inline unsigned & operator<<= (unsigned & u, const QVariant & v)
{
	u = v.toUInt(); return u;
}

#endif
