/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CONVERSIONS_H
#define BUTLER_CONVERSIONS_H

#include <csjp_string.h>
#include <butler_text.h>
#include <butler_datetime.h>
#include <butler_dataclasses.h>

#include <QString>
#include <QVariant>

inline csjp::String & operator<<= (csjp::String & str, const QString & qstr)
{
	str = C_STR(qstr); return str;
}

inline csjp::String & operator<<= (csjp::String & str, unsigned u)
{
	str.cutAt(0); str << u; return str;
}

inline QString & operator<<= (QString & qstr, const csjp::String & str)
{
	qstr = str.c_str(); return qstr;
}

inline QString & operator<<= (QString & qstr, const QVariant & v)
{
	qstr = v.toString(); return qstr;
}

inline DateTime & operator<<= (DateTime & time, const QVariant & v)
{
	time = v.toDateTime();
	return time;
}

inline int & operator<<= (int & i, const QVariant & v)
{
	i = v.toInt(); return i;
}

inline unsigned & operator<<= (unsigned & u, const QVariant & v)
{
	u = v.toUInt(); return u;
}

inline double & operator<<= (double & d, const QVariant & v)
{
	d = v.toDouble(); return d;
}

inline bool & operator<<= (bool & b, const QVariant & v)
{
	b = v.toBool(); return b;
}

#endif
