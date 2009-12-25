/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_debug.h"

#include "butler_tag_set.h"

namespace Butler
{

	TagSet::TagSet()
	{
		ENTER_FUNCTION();
		LEAVE_FUNCTION();
	}

	TagSet::~TagSet()
	{
		clear();
	}

	TagSet::TagSet(const TagSet &ts) : QObject()
	{
		equal(ts);
	}
			
	TagSet& TagSet::operator=(const TagSet& ts)
	{
		clear();
		equal(ts);
		return *this;
	}

	void TagSet::append(Tag *t)
	{
		Q_ASSERT(t);
		data.append(t);
		nameToPtr.insert(&(t->name), t);
	}

	void TagSet::remove(int i)
	{
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		Tag *t = data.at(i);
		int r;
		r = nameToPtr.remove(&(data.at(i)->name));
		Q_ASSERT(r == 1);
		data.removeAt(i);
		delete t;
	}

	void TagSet::clear()
	{
		int s = data.size();
		int i;
		for(i=0; i < s; i++){
			Tag *t = data.at(i);
			delete t;
		}
		nameToPtr.clear();
		data.clear();
	}

	void TagSet::move(int from, int to)
	{
		Q_ASSERT(from < data.size());
		Q_ASSERT(0 <= from);
		Q_ASSERT(to < data.size());
		Q_ASSERT(0 <= to);
		data.move(from, to);
	}

	void TagSet::swap(int i, int j)
	{
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		Q_ASSERT(j < data.size());
		Q_ASSERT(0 <= j);
		data.swap(i, j);
	}

	const Tag& TagSet::query(int i) const
	{
		Q_ASSERT(i<data.size());
		return *(data.at(i));
	}

	bool TagSet::empty() const
	{
		return data.isEmpty();
	}

	int TagSet::size() const
	{
		return data.size();
	}

	void TagSet::sort()
	{
		qSort(data.begin(), data.end(), TagSet::qSortIsLess);
	}

	bool TagSet::isEqual(const TagSet &a, const TagSet &b)
	{
		bool ret = true;
		int s = a.size();

		if(s != b.size())
			ret = false;
		else {
			int i;
			for(i=0; i < s; i++){
				const Tag &ta = a.query(i);
				const Tag &tb = b.query(i);
				if(ta != tb){
					ret = false;
					break;
				}
			}
		}

		return ret;
	}
			
	void TagSet::equal(const TagSet &ts)
	{
		int s = ts.size();
		int i;
		for(i=0; i < s; i++){
			Tag *t = new Tag(ts.query(i));
			data.append(t);
			nameToPtr.insert(&(t->name), t);
		}
	}

	bool TagSet::qSortIsLess(const Tag* s1, const Tag* s2)
	{
		return *s1 < *s2;
	}

	bool operator==(const TagSet &a, const TagSet &b)
	{
		return TagSet::isEqual(a,b);
	}
	
	bool operator!=(const TagSet &a, const TagSet &b)
	{
		return !TagSet::isEqual(a,b);
	}
}

