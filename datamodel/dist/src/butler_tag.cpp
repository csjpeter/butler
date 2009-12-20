/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_tag.h"

namespace Butler {

	Tag::Tag()
	{
	}

	Tag::Tag(const QString &_name)
	{
		name = _name;
		checked = false;
	}

	Tag::Tag(const Tag &tag) : QObject()
	{
		name = tag.name;
		checked = tag.checked;
	}

	Tag::~Tag()
	{
	}

	bool Tag::isEqual(const Tag &a, const Tag &b)
	{
		return a.name == b.name;
	}

	bool Tag::isEqual(const Tag &a, const QString &str)
	{
		return a.name == str;
	}

	bool Tag::isEqual(const QString &str, const Tag &b)
	{
		return str == b.name;
	}
			
	bool isLess(const Tag &a, const Tag &b)
	{
		return QString::localeAwareCompare(a.name, b.name) < 0;
	}

	bool operator==(const Tag &a, const Tag &b)
	{
		return Tag::isEqual(a,b);
	}

	bool operator==(const Tag &a, const QString &str)
	{
		return Tag::isEqual(a,str);
	}

	bool operator==(const QString &str, const Tag &b)
	{
		return Tag::isEqual(str,b);
	}

	bool operator<(const Tag &a, const Tag &b)
	{
		return Tag::isLess(a, b);
	}
}

