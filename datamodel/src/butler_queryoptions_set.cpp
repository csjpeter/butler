/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_debug.h"

#include "butler_queryoptions_set.h"

namespace Butler
{

	QueryOptionsSet::QueryOptionsSet()
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	QueryOptionsSet::~QueryOptionsSet()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	QueryOptionsSet::QueryOptionsSet(const QueryOptionsSet &qos) :
		QObject(), OrderedSet<QueryOptions>(qos)
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}
			
	QueryOptionsSet& QueryOptionsSet::operator=(const QueryOptionsSet& ts)
	{
		ENTER_FUNCTION();
		clear();
		equal(ts);
		LEAVE_FUNCTION();
		return *this;
	}

	void QueryOptionsSet::append(QueryOptions *t)
	{
		ENTER_FUNCTION();
		OrderedSet<QueryOptions>::append(t);
		nameToPtr.insert(&(t->name), t);
		LEAVE_FUNCTION();
	}

	void QueryOptionsSet::remove(int i)
	{
		ENTER_FUNCTION();
		Q_ASSERT(i < size());
		Q_ASSERT(0 <= i);
		int r;
		r = nameToPtr.remove(&(query(i).name));
		Q_ASSERT(r == 1);
		OrderedSet<QueryOptions>::remove(i);
		LEAVE_FUNCTION();
	}

	void QueryOptionsSet::clear()
	{
		ENTER_FUNCTION();
		nameToPtr.clear();
		OrderedSet<QueryOptions>::clear();
		LEAVE_FUNCTION();
	}

}

