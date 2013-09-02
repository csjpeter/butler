/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_TEXT_H
#define BUTLER_TEXT_H

#include <csjp_string.h>

namespace csjp {

class Text : public QString
{
public:
		Text() : QString() {}
		explicit Text(const csjp::Text & t) : QString(t) {}
		Text(const csjp::Text && t) : QString(csjp::move_cast(t)) {}
		explicit Text(const char * s) : QString(s) {}
		explicit Text(const unsigned short * utf16) : QString(QString::fromUtf16(utf16)) {}
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

		bool isEqual(const Text & t) const
		{
				return QString::localeAwareCompare(*this, t) == 0;
		}

		bool isLess(const Text & t) const
		{
				return QString::localeAwareCompare(*this, t) < 0;
		}

		bool isMore(const Text & t) const
		{
				return 0 < QString::localeAwareCompare(*this, t);
		}

private:
		void copy(const Text & t)
		{
				QString::operator=(t);
		}
};

inline bool operator==(const Text & a, const Text & b)
{
		return a.isEqual(b);
}

inline bool operator!=(const Text & a, const Text & b)
{
		return !a.isEqual(b);
}

inline bool operator<(const Text & a, const Text & b)
{
		return a.isLess(b);
}

};

#endif
