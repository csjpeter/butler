/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_CONVERSIONS_H
#define BUTLER_CONVERSIONS_H

#include <csjp_exception.h>
#include <csjp_string.h>
#include <butler_types.h>
#include <butler_text.h>
#include <butler_datetime.h>
#include <butler_dataclasses.h>

#include <QString>
#include <QVariant>

inline QString & operator<<= (QString & qstr, const enum QueryStockOptions & rhs)
{
	switch(rhs)
	{
		case QueryStockOptions::AllItemChanges :
			qstr = "AllItemChanges";
			break;
		case QueryStockOptions::Gains :
			qstr = "Gains";
			break;
		case QueryStockOptions::Looses :
			qstr = "Looses";
			break;
		default:
			throw csjp::InvalidArgument();
	}
	return qstr;
}

inline enum QueryStockOptions & operator<<= (enum QueryStockOptions & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "AllItemChanges")
				lhs = QueryStockOptions::AllItemChanges;
		else if(s == "Gains")
				lhs = QueryStockOptions::Gains;
		else if(s == "Looses")
				lhs = QueryStockOptions::Looses;
		else
			throw csjp::InvalidArgument();
	return lhs;
}

inline QString & operator<<= (QString & qstr, const enum QueryTagOptions & rhs)
{
	switch(rhs)
	{
		case QueryTagOptions::MatchAll :
			qstr = "MatchAll";
			break;
		case QueryTagOptions::MatchAny :
			qstr = "MatchAny";
			break;
		default:
			throw csjp::InvalidArgument();
	}
	return qstr;
}

inline enum QueryTagOptions & operator<<= (enum QueryTagOptions & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "MatchAll")
				lhs = QueryTagOptions::MatchAll;
		else if(s == "MatchAny")
				lhs = QueryTagOptions::MatchAny;
		else
			throw csjp::InvalidArgument();
	return lhs;
}

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

inline QVariant & operator<<= (QVariant & var, const csjp::String & rhs)
{
	QVariant v(QString(rhs.str));
	var = v;
	return var;
}

inline QVariant & operator<<= (QVariant & var, const bool & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const int & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const unsigned & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const double & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const QString & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const QDateTime & rhs)
{
	var.setValue(rhs);
	return var;
}

inline csjp::String & operator<<= (csjp::String & str, const QVariant & v)
{
	str = C_STR(v.toString()); return str;
}

inline QDateTime & operator<<= (QDateTime & lhs, const QVariant & v)
{
	lhs = v.toDateTime(); return lhs;
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
