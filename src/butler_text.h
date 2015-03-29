/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_TEXT_H
#define BUTLER_TEXT_H

#include <csjp_string.h>

#include <QString>
#include <QVariant>

#include <butler_macros.h>

class Text : public QString
{
public:
		Text() : QString() {}
		explicit Text(const QVariant & v) : QString(v.toString()) {}
		explicit Text(const QString & t) : QString(t) {}
		Text(QString && t) : QString(csjp::move_cast(t)) {}
		explicit Text(const Text & t) : QString(t) {}
		Text(Text && t) : QString(csjp::move_cast(t)) {}
		explicit Text(const char * s) : QString(s) {}
		explicit Text(const csjp::String & t) : QString(t.c_str()) {}
		virtual ~Text() {}

		Text & operator=(const char * s) { QString::operator=(s); return *this; }
		Text & operator=(const Text & t) { copy(t); return *this; }

		bool isEqual(const QString & t) const { return QString::localeAwareCompare(*this, t) == 0; }
		bool isLess(const QString & t) const { return QString::localeAwareCompare(*this, t) < 0; }
		bool isMore(const QString & t) const { return 0 < QString::localeAwareCompare(*this, t); }

private:
		void copy(const QString & t) { QString::operator=(t); }
};

inline bool operator==(const Text & a, const QString & b) { return a.isEqual(b); }
inline bool operator==(const Text & a, const Text & b) { return a.isEqual(b); }
inline bool operator!=(const Text & a, const QString & b) { return !a.isEqual(b); }
inline bool operator!=(const QString & a, const Text & b) { return !b.isEqual(a); }
inline bool operator!=(const Text & a, const Text & b) { return !a.isEqual(b); }
inline bool operator<(const Text & a, const QString & b) { return a.isLess(b); }
inline bool operator<(const QString & a, const Text & b) { return b.isMore(a); }
inline bool operator<(const Text & a, const Text & b) { return a.isLess(b); }

inline Text & operator<<= (Text & str, const char * rhs) { str = rhs; return str; }
inline Text & operator<<= (Text & str, const csjp::CString & rhs) { str = rhs.ptr; return str; }

inline csjp::String & operator<< (csjp::String & str, const Text & rhs)
{ str.append(C_STR(rhs)); return str; }

#endif
