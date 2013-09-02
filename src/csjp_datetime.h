/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_DATETIME_H
#define BUTLER_DATETIME_H

#include <csjp_string.h>

#include <QDateTime>

namespace csjp {

class DateTime
{
public:
		DateTime() : time(QDate(0,0,0), QTime(0,0,0)) {}
		explicit DateTime(const csjp::DateTime & t) : time(t.time) {}
		DateTime(const csjp::DateTime && t) : time(csjp::move_cast(t.time)) {}
		explicit DateTime(const char * s) : time(QDateTime::fromString(QString(s), Qt::ISODate)) {}
		explicit DateTime(const unsigned short * utf16) :
				time(QDateTime::fromString(QString::fromUtf16(utf16), Qt::ISODate)) {}
		explicit DateTime(const csjp::String & t) :
				time(QDateTime::fromString(QString(t.c_str()), Qt::ISODate)) {}
		virtual ~DateTime() {}

		DateTime & operator=(const unsigned short * utf16)
		{
				time = QDateTime::fromString(QString::fromUtf16(utf16), Qt::ISODate);
				return *this;
		}

		DateTime & operator=(const char * s)
		{
				time = QDateTime::fromString(QString(s), Qt::ISODate);
				return *this;
		}

		DateTime & operator=(const DateTime & t)
		{
				copy(t);
				return *this;
		}

		bool isEqual(const DateTime & t) const
		{
				return time.toString() == t.time.toString();
		}

		bool isLess(const DateTime & t) const
		{
				return time < t.time;
		}

		bool isMore(const DateTime & t) const
		{
				return t.time < time;
		}

private:
		void copy(const DateTime & t)
		{
				time = t.time;
		}

private:
		QDateTime time;
};

inline bool operator==(const DateTime & a, const DateTime & b)
{
		return a.isEqual(b);
}

inline bool operator!=(const DateTime & a, const DateTime & b)
{
		return !a.isEqual(b);
}

inline bool operator<(const DateTime & a, const DateTime & b)
{
		return a.isLess(b);
}

};

#endif
