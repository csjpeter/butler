/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_TEXT_H
#define BUTLER_TEXT_H

#include <csjp_string.h>

#include <QString>
#include <QVariant>

class Text : public QString
{
public:
		Text() : QString() {}
		explicit Text(const QVariant & v) : QString(v.toString()) {}
		explicit Text(const QString & t) : QString(t) {}
		Text(const QString && t) : QString(csjp::move_cast(t)) {}
		explicit Text(const Text & t) : QString(t) {}
		Text(const Text && t) : QString(csjp::move_cast(t)) {}
		explicit Text(const char * s) : QString(s) {}
		explicit Text(const csjp::String & t) : QString(t.c_str()) {}
		virtual ~Text() {}

		Text & operator=(const unsigned short * utf16)
		{
				*(QString*)this = QString::fromUtf16(utf16);
				return *this;
		}

		Text & operator=(const char * s)
		{
				QString::operator=(s);
				return *this;
		}

		Text & operator=(const Text & t)
		{
				copy(t);
				return *this;
		}

		bool isEqual(const QString & t) const
		{
				return QString::localeAwareCompare(*this, t) == 0;
		}

		bool isLess(const QString & t) const
		{
				return QString::localeAwareCompare(*this, t) < 0;
		}

		bool isMore(const QString & t) const
		{
				return 0 < QString::localeAwareCompare(*this, t);
		}

private:
		void copy(const QString & t)
		{
				QString::operator=(t);
		}
};

inline bool operator==(const Text & a, const QString & b)
{
		return a.isEqual(b);
}

inline bool operator==(const Text & a, const Text & b)
{
		return a.isEqual(b);
}

inline bool operator!=(const Text & a, const QString & b)
{
		return !a.isEqual(b);
}

inline bool operator!=(const QString & a, const Text & b)
{
		return !b.isEqual(a);
}

inline bool operator!=(const Text & a, const Text & b)
{
		return !a.isEqual(b);
}

inline bool operator<(const Text & a, const Text & b)
{
		return a.isLess(b);
}

#endif
