/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

