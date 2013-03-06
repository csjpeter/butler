/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_tag.h"

Tag::Tag() :
	name(""),
	description("")
{
}

Tag::Tag(const QString &_name) :
	description("")
{
	name = _name;
}

Tag::Tag(const Tag &tag)
{
	equal(tag);
}

Tag::~Tag()
{
}
		
Tag& Tag::operator=(const Tag& tag)
{
	equal(tag);
	return *this;
}

bool Tag::isEqual(const Tag &t) const
{
	return (name == t.name) &&
		(description == t.description);
}
		
bool Tag::isLess(const Tag &t) const
{
	return QString::localeAwareCompare(name, t.name) < 0;
}

bool Tag::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool Tag::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}
		
void Tag::equal(const Tag &tag)
{
	name = tag.name;
	description = tag.description;
}

bool operator==(const Tag &a, const Tag &b)
{
	return a.isEqual(b);
}

bool operator!=(const Tag &a, const Tag &b)
{
	return !a.isEqual(b);
}

bool operator<(const Tag &a, const Tag &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const Tag &b)
{
	return b.isMore(a);
}

bool operator<(const Tag &a, const QString &b)
{
	return a.isLess(b);
}
