/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_queryoptions.h"

namespace Butler {

	QueryOptions::QueryOptions()
	{
	}

	QueryOptions::QueryOptions(const QString &_name) : QObject()
	{
		name = _name;
	}

	QueryOptions::QueryOptions(const QueryOptions &qo) : QObject()
	{
		equal(qo);
	}

	QueryOptions::~QueryOptions()
	{
	}

	QueryOptions& QueryOptions::operator=(const QueryOptions& qo)
	{
		equal(qo);
		return *this;
	}

	bool QueryOptions::isEqual(const QueryOptions &a, const QueryOptions &b)
	{
		bool ret = true;
		if(a.name != b.name)
			ret = false;
		if(a.tags != b.tags)
			ret = false;
		return ret;
	}
			
	bool QueryOptions::isLess(const QueryOptions &a, const QueryOptions &b)
	{
		return QString::localeAwareCompare(a.name, b.name) < 0;
	}

	bool QueryOptions::isMore(const QueryOptions &a, const QueryOptions &b)
	{
		return 0 < QString::localeAwareCompare(a.name, b.name);
	}

	void QueryOptions::equal(const QueryOptions &qo)
	{
		name = qo.name;
		tags = qo.tags;
	}

	bool operator==(const QueryOptions &a, const QueryOptions &b)
	{
		return QueryOptions::isEqual(a,b);
	}

	bool operator!=(const QueryOptions &a, const QueryOptions &b)
	{
		return !QueryOptions::isEqual(a,b);
	}
	
	bool operator<(const QueryOptions &a, const QueryOptions &b)
	{
		return QueryOptions::isLess(a,b);
	}

	bool operator>(const QueryOptions &a, const QueryOptions &b)
	{
		return QueryOptions::isMore(a,b);
	}
}

