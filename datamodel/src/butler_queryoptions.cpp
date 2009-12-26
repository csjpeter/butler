/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_debug.h"

#include "butler_queryoptions.h"

namespace Butler {

	QueryOptions::QueryOptions()
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	QueryOptions::QueryOptions(const QString &_name) : QObject()
	{
		ENTER_CONSTRUCTOR();
		name = _name;
		LEAVE_CONSTRUCTOR();
	}

	QueryOptions::QueryOptions(const QueryOptions &qo) : QObject()
	{
		ENTER_CONSTRUCTOR();
		equal(qo);
		LEAVE_CONSTRUCTOR();
	}

	QueryOptions::~QueryOptions()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	QueryOptions& QueryOptions::operator=(const QueryOptions& qo)
	{
		ENTER_FUNCTION();
		equal(qo);
		LEAVE_FUNCTION();
		return *this;
	}

	bool QueryOptions::isEqual(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = true;
		if(a.name != b.name)
			ret = false;
		if(a.tags != b.tags)
			ret = false;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
			
	bool QueryOptions::isLess(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = QString::localeAwareCompare(a.name, b.name) < 0;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool QueryOptions::isMore(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = 0 < QString::localeAwareCompare(a.name, b.name);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	void QueryOptions::equal(const QueryOptions &qo)
	{
		ENTER_FUNCTION();
		name = qo.name;
		tags = qo.tags;
		LEAVE_FUNCTION();
	}

	bool operator==(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = QueryOptions::isEqual(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator!=(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = !QueryOptions::isEqual(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
	
	bool operator<(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = QueryOptions::isLess(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator>(const QueryOptions &a, const QueryOptions &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = QueryOptions::isMore(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
}

