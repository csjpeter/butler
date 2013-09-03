/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_DATETIME_H
#define BUTLER_DATETIME_H

#include <butler_text.h>

#include <QDateTime>

#include <butler_macros.h>

class DateTime : public QDateTime
{
public:
		DateTime() : QDateTime(QDate(0,0,0), QTime(0,0,0)) {}
		explicit DateTime(const QVariant & v) : QDateTime(v.toDateTime()) {}
		explicit DateTime(const QDateTime & t) : QDateTime(t) {}
		explicit DateTime(const DateTime & t) : QDateTime(t) {}
		DateTime(const DateTime && t) : QDateTime(csjp::move_cast(t)) {}
		explicit DateTime(const char * s) :
				QDateTime(QDateTime::fromString(QString(s), Qt::ISODate)) {}
		explicit DateTime(const csjp::String & t) :
				QDateTime(QDateTime::fromString(QString(t.c_str()), Qt::ISODate)) {}
		virtual ~DateTime() {}

		DateTime & operator=(const char * s)
		{
				QDateTime & dt = *this;
				dt = QDateTime::fromString(QString(s), Qt::ISODate);
				return *this;
		}

		DateTime & operator=(const QDateTime & t)
		{
				copy(t);
				return *this;
		}

		DateTime & operator=(const DateTime & t)
		{
				copy(t);
				return *this;
		}

		bool isEqual(const QDateTime & t) const
		{
				return toString() == t.toString();
		}

		bool isLess(const QDateTime & t) const
		{
				return *this < t;
		}

		bool isMore(const QDateTime & t) const
		{
				return t < *this;
		}

		/* These methods are providing and expecting only the
		 * the ISO:8601 compatible YYYY-MM-DDTHH:MM:SS format. */
		QString isoUtcString() const
		{
				return toUTC().toString("yyyy-MM-ddThh:mm:ssZ");
		}

private:
		void copy(const QDateTime & t)
		{
				QDateTime & dt = *this;
				dt = t;
		}
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

#endif
