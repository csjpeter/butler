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
		ENTER_FUNCTION();
		LEAVE_FUNCTION();
	}

	QueryOptionsSet::~QueryOptionsSet()
	{
		clear();
	}

	QueryOptionsSet::QueryOptionsSet(const QueryOptionsSet &ts) : QObject()
	{
		equal(ts);
	}
			
	QueryOptionsSet& QueryOptionsSet::operator=(const QueryOptionsSet& ts)
	{
		clear();
		equal(ts);
		return *this;
	}

	void QueryOptionsSet::append(QueryOptions *t)
	{
		Q_ASSERT(t);
		data.append(t);
		nameToPtr.insert(&(t->name), t);
	}

	void QueryOptionsSet::remove(int i)
	{
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		QueryOptions *qo = data.at(i);
		int r;
		r = nameToPtr.remove(&(data.at(i)->name));
		Q_ASSERT(r == 1);
		data.removeAt(i);
		delete qo;
	}

	void QueryOptionsSet::clear()
	{
		int s = data.size();
		int i;
		for(i=0; i < s; i++){
			QueryOptions *qo = data.at(i);
			delete qo;
		}
		nameToPtr.clear();
		data.clear();
	}

	void QueryOptionsSet::move(int from, int to)
	{
		Q_ASSERT(from < data.size());
		Q_ASSERT(0 <= from);
		Q_ASSERT(to < data.size());
		Q_ASSERT(0 <= to);
		data.move(from, to);
	}

	void QueryOptionsSet::swap(int i, int j)
	{
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		Q_ASSERT(j < data.size());
		Q_ASSERT(0 <= j);
		data.swap(i, j);
	}

	const QueryOptions& QueryOptionsSet::query(int i) const
	{
		Q_ASSERT(i<data.size());
		return *(data.at(i));
	}

	bool QueryOptionsSet::empty() const
	{
		return data.isEmpty();
	}

	int QueryOptionsSet::size() const
	{
		return data.size();
	}

	void QueryOptionsSet::sort()
	{
		qSort(data.begin(), data.end(), QueryOptionsSet::qSortIsLess);
	}

	bool QueryOptionsSet::isEqual(const QueryOptionsSet &a, const QueryOptionsSet &b)
	{
		bool ret = true;
		int s = a.size();

		if(s != b.size())
			ret = false;
		else {
			int i;
			for(i=0; i < s; i++){
				const QueryOptions &ta = a.query(i);
				const QueryOptions &tb = b.query(i);
				if(ta != tb){
					ret = false;
					break;
				}
			}
		}

		return ret;
	}
			
	void QueryOptionsSet::equal(const QueryOptionsSet &ts)
	{
		int s = ts.size();
		int i;
		for(i=0; i < s; i++){
			QueryOptions *qo = new QueryOptions(ts.query(i));
			data.append(qo);
			nameToPtr.insert(&(qo->name), qo);
		}
	}

	bool QueryOptionsSet::qSortIsLess(const QueryOptions* s1, const QueryOptions* s2)
	{
		return *s1 < *s2;
	}

	bool operator==(const QueryOptionsSet &a, const QueryOptionsSet &b)
	{
		return QueryOptionsSet::isEqual(a,b);
	}
	
	bool operator!=(const QueryOptionsSet &a, const QueryOptionsSet &b)
	{
		return !QueryOptionsSet::isEqual(a,b);
	}
}

