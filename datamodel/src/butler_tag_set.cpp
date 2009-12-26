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
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	TagSet::~TagSet()
	{
		ENTER_DESTRUCTOR();
		clear();
		LEAVE_DESTRUCTOR();
	}

	TagSet::TagSet(const TagSet &ts) : QObject()
	{
		ENTER_CONSTRUCTOR();
		equal(ts);
		LEAVE_CONSTRUCTOR();
	}
			
	TagSet& TagSet::operator=(const TagSet& ts)
	{
		ENTER_FUNCTION();
		clear();
		equal(ts);
		LEAVE_FUNCTION();
		return *this;
	}

	void TagSet::append(Tag *t)
	{
		ENTER_FUNCTION();
		Q_ASSERT(t);
		data.append(t);
		nameToPtr.insert(&(t->name), t);
		LEAVE_FUNCTION();
	}

	void TagSet::remove(int i)
	{
		ENTER_FUNCTION();
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		Tag *t = data.at(i);
		int r;
		r = nameToPtr.remove(&(data.at(i)->name));
		Q_ASSERT(r == 1);
		data.removeAt(i);
		delete t;
		LEAVE_FUNCTION();
	}

	void TagSet::clear()
	{
		ENTER_FUNCTION();
		int s = data.size();
		int i;
		for(i=0; i < s; i++){
			Tag *t = data.at(i);
			delete t;
		}
		nameToPtr.clear();
		data.clear();
		LEAVE_FUNCTION();
	}

	void TagSet::move(int from, int to)
	{
		ENTER_FUNCTION();
		Q_ASSERT(from < data.size());
		Q_ASSERT(0 <= from);
		Q_ASSERT(to < data.size());
		Q_ASSERT(0 <= to);
		data.move(from, to);
		LEAVE_FUNCTION();
	}

	void TagSet::swap(int i, int j)
	{
		ENTER_FUNCTION();
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		Q_ASSERT(j < data.size());
		Q_ASSERT(0 <= j);
		data.swap(i, j);
		LEAVE_FUNCTION();
	}

	const Tag& TagSet::query(int i) const
	{
		ENTER_FUNCTION();
		Q_ASSERT(i<data.size());
		Tag *t = data.at(i);
		Q_ASSERT(t != NULL);
		LEAVE_FUNCTION();
		return *t;
	}

	bool TagSet::empty() const
	{
		ENTER_FUNCTION();
		bool ret = data.isEmpty();
		LEAVE_FUNCTION();
		return ret;
	}

	int TagSet::size() const
	{
		ENTER_FUNCTION();
		int ret = data.size();
		LEAVE_FUNCTION();
		return ret;
	}

	void TagSet::sort()
	{
		ENTER_FUNCTION();
		qSort(data.begin(), data.end(), TagSet::qSortIsLess);
		LEAVE_FUNCTION();
	}

	bool TagSet::isEqual(const TagSet &a, const TagSet &b)
	{
		ENTER_STATIC_FUNCTION();
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

		LEAVE_STATIC_FUNCTION();
		return ret;
	}
			
	void TagSet::equal(const TagSet &ts)
	{
		ENTER_FUNCTION();
		int s = ts.size();
		int i;
		for(i=0; i < s; i++){
			Tag *t = new Tag(ts.query(i));
			data.append(t);
			nameToPtr.insert(&(t->name), t);
		}
		LEAVE_FUNCTION();
	}

	bool TagSet::qSortIsLess(const Tag* s1, const Tag* s2)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = *s1 < *s2;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	bool operator==(const TagSet &a, const TagSet &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = TagSet::isEqual(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
	
	bool operator!=(const TagSet &a, const TagSet &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = !TagSet::isEqual(a,b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
}

