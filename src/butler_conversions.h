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


inline csjp::String & operator<<=(csjp::String & lhs, unsigned rhs)
		{ lhs.cutAt(0); lhs << rhs; return lhs; }

inline csjp::String & operator<<=(csjp::String & lhs, const QString & rhs)
		{ lhs = C_STR(rhs); return lhs; }
inline QString & operator<<=(QString & lhs, const csjp::String & rhs)
		{ lhs = rhs.c_str(); return lhs; }

inline csjp::String & operator<<(csjp::String & lhs, const QString & rhs)
		{ lhs += C_STR(rhs); return lhs; }

inline QVariant & operator<<=(QVariant & lhs, const bool & rhs) { lhs.setValue(rhs); return lhs; }
inline QVariant & operator<<=(QVariant & lhs, const int & rhs) { lhs.setValue(rhs); return lhs; }
inline QVariant & operator<<=(QVariant & lhs, const unsigned & rhs) {lhs.setValue(rhs); return lhs;}
inline QVariant & operator<<=(QVariant & lhs, const double & rhs) { lhs.setValue(rhs); return lhs; }
inline QVariant & operator<<=(QVariant & lhs, const QDateTime & rhs){lhs.setValue(rhs); return lhs;}
inline QVariant & operator<<=(QVariant & lhs, const QString & rhs) {lhs.setValue(rhs); return lhs;}
inline QVariant & operator<<=(QVariant & lhs, const csjp::String & rhs)
		{ QVariant v(QString(rhs.c_str())); lhs = v; return lhs; }


inline bool & operator<<=(bool & lhs, const QVariant & rhs) { lhs = rhs.toBool(); return lhs; }
inline int & operator<<=(int & lhs, const QVariant & rhs) { lhs = rhs.toInt(); return lhs; }
inline unsigned & operator<<=(unsigned & lhs, const QVariant & rhs){lhs = rhs.toUInt(); return lhs;}
inline double & operator<<=(double & lhs, const QVariant & rhs){ lhs = rhs.toDouble(); return lhs; }
inline QDateTime & operator<<=(QDateTime & lhs, const QVariant & v){lhs=v.toDateTime(); return lhs;}
inline QString & operator<<=(QString & lhs, const QVariant & rhs){lhs = rhs.toString(); return lhs;}
inline csjp::String & operator<<=(csjp::String & lhs, const QVariant & rhs)
		{ lhs = C_STR(rhs.toString()); return lhs; }

#endif
