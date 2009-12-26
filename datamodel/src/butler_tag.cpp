/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_debug.h"

#include "butler_tag.h"

namespace Butler {

	Tag::Tag()
	{
		ENTER_CONSTRUCTOR();
		checked = false;
		LEAVE_CONSTRUCTOR();
	}

	Tag::Tag(const QString &_name)
	{
		ENTER_CONSTRUCTOR();
		name = _name;
		checked = false;
		LEAVE_CONSTRUCTOR();
	}

	Tag::Tag(const Tag &tag) : QObject()
	{
		ENTER_CONSTRUCTOR();
		equal(tag);
		LEAVE_CONSTRUCTOR();
	}

	Tag::~Tag()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}
			
	Tag& Tag::operator=(const Tag& tag)
	{
		ENTER_FUNCTION();
		equal(tag);
		LEAVE_FUNCTION();
		return *this;
	}

	bool Tag::isEqual(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = (a.name == b.name) && (a.checked == b.checked);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
			
	bool Tag::isLess(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = QString::localeAwareCompare(a.name, b.name) < 0;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool Tag::isMore(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = 0 < QString::localeAwareCompare(a.name, b.name);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
			
	void Tag::equal(const Tag &tag)
	{
		ENTER_FUNCTION();
		name = tag.name;
		checked = tag.checked;
		LEAVE_FUNCTION();
	}

	bool operator==(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = Tag::isEqual(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator!=(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = !Tag::isEqual(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator<(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = Tag::isLess(a, b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator>(const Tag &a, const Tag &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = Tag::isMore(a, b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
}

