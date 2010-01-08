/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_debug.h"

#include "butler_query.h"

namespace Butler {

	Query::Query()
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	Query::Query(const QString &_name) : QObject()
	{
		ENTER_CONSTRUCTOR();
		name = _name;
		LEAVE_CONSTRUCTOR();
	}

	Query::Query(const Query &qo) : QObject()
	{
		ENTER_CONSTRUCTOR();
		equal(qo);
		LEAVE_CONSTRUCTOR();
	}

	Query::~Query()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	Query& Query::operator=(const Query& qo)
	{
		ENTER_FUNCTION();
		equal(qo);
		LEAVE_FUNCTION();
		return *this;
	}

	bool Query::isEqual(const Query &qo) const
	{
		ENTER_STATIC_FUNCTION();
		bool ret = true;
		if(name != qo.name)
			ret = false;
		if(startDate != qo.startDate)
			ret = false;
		if(endDate != qo.endDate)
			ret = false;
		if(tags != qo.tags)
			ret = false;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
			
	bool Query::isLess(const Query &qo) const
	{
		ENTER_STATIC_FUNCTION();
		bool ret = QString::localeAwareCompare(name, qo.name) < 0;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool Query::isMore(const Query &qo) const
	{
		ENTER_STATIC_FUNCTION();
		bool ret = 0 < QString::localeAwareCompare(name, qo.name);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	void Query::equal(const Query &qo)
	{
		ENTER_FUNCTION();
		name = qo.name;
		startDate = qo.startDate;
		endDate = qo.endDate;
		tags = qo.tags;
		LEAVE_FUNCTION();
	}

	bool operator==(const Query &a, const Query &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator!=(const Query &a, const Query &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = !a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
	
	bool operator<(const Query &a, const Query &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = a.isLess(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator>(const Query &a, const Query &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = a.isMore(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
}

