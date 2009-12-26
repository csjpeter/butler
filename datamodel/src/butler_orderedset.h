/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef ORDEREDSET_H
#define ORDEREDSET_H

#include <QList>

#include "butler_debug.h"

namespace Butler {

	template <typename Type>
	class OrderedSet
	{
		public:
			OrderedSet();
			virtual ~OrderedSet();

			explicit OrderedSet(const OrderedSet &set);
			OrderedSet& operator=(const OrderedSet &set);

			void append(Type *t);
			void remove(int i);
			void clear();
			void move(int from, int to);

			const Type& query(int i) const;

			bool empty() const;
			int size() const;

			void sort();

			bool isEqual(const OrderedSet &set) const;

		protected:
			void equal(const OrderedSet &s);
		
		private:
			static bool qSortIsLess(
					const Type* s1, const Type* s2);

			QList<Type*> data;
	};

	template <typename Type> bool operator==(const OrderedSet<Type> &a, const OrderedSet<Type> &b);
	template <typename Type> bool operator!=(const OrderedSet<Type> &a, const OrderedSet<Type> &b);
	
	
	
	
	
	template <typename Type> OrderedSet<Type>::OrderedSet()
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	template <typename Type> OrderedSet<Type>::~OrderedSet()
	{
		ENTER_DESTRUCTOR();
		clear();
		LEAVE_DESTRUCTOR();
	}

	template <typename Type> OrderedSet<Type>::OrderedSet(const OrderedSet &ts)
	{
		ENTER_CONSTRUCTOR();
		equal(ts);
		LEAVE_CONSTRUCTOR();
	}
			
	template <typename Type> OrderedSet<Type>& OrderedSet<Type>::operator=(const OrderedSet& ts)
	{
		ENTER_FUNCTION();
		clear();
		equal(ts);
		LEAVE_FUNCTION();
		return *this;
	}

	template <typename Type> void OrderedSet<Type>::append(Type *t)
	{
		ENTER_FUNCTION();
		Q_ASSERT(t);
		data.append(t);
		LEAVE_FUNCTION();
	}

	template <typename Type> void OrderedSet<Type>::remove(int i)
	{
		ENTER_FUNCTION();
		Q_ASSERT(i < data.size());
		Q_ASSERT(0 <= i);
		Type *t = data.at(i);
		data.removeAt(i);
		delete t;
		LEAVE_FUNCTION();
	}

	template <typename Type> void OrderedSet<Type>::clear()
	{
		ENTER_FUNCTION();
		int s = data.size();
		int i;
		for(i=0; i < s; i++){
			Type *t = data.at(i);
			delete t;
		}
		data.clear();
		LEAVE_FUNCTION();
	}

	template <typename Type> void OrderedSet<Type>::move(int from, int to)
	{
		ENTER_FUNCTION();
		Q_ASSERT(from < data.size());
		Q_ASSERT(0 <= from);
		Q_ASSERT(to < data.size());
		Q_ASSERT(0 <= to);
		data.move(from, to);
		LEAVE_FUNCTION();
	}

	template <typename Type> const Type& OrderedSet<Type>::query(int i) const
	{
		ENTER_FUNCTION();
		Q_ASSERT(i<data.size());
		Type *t = data.at(i);
		Q_ASSERT(t != NULL);
		LEAVE_FUNCTION();
		return *t;
	}

	template <typename Type> bool OrderedSet<Type>::empty() const
	{
		ENTER_FUNCTION();
		bool ret = data.isEmpty();
		LEAVE_FUNCTION();
		return ret;
	}

	template <typename Type> int OrderedSet<Type>::size() const
	{
		ENTER_FUNCTION();
		int ret = data.size();
		LEAVE_FUNCTION();
		return ret;
	}

	template <typename Type> void OrderedSet<Type>::sort()
	{
		ENTER_FUNCTION();
		qSort(data.begin(), data.end(), OrderedSet::qSortIsLess);
		LEAVE_FUNCTION();
	}

	template <typename Type> bool OrderedSet<Type>::isEqual(const OrderedSet &set) const
	{
		ENTER_STATIC_FUNCTION();
		bool ret = true;
		int s = size();

		if(s != set.size())
			ret = false;
		else {
			int i;
			for(i=0; i < s; i++){
				const Type &ta = query(i);
				const Type &tb = set.query(i);
				if(ta != tb){
					ret = false;
					break;
				}
			}
		}

		LEAVE_STATIC_FUNCTION();
		return ret;
	}
			
	template <typename Type> void OrderedSet<Type>::equal(const OrderedSet &ts)
	{
		ENTER_FUNCTION();
		int s = ts.size();
		int i;
		for(i=0; i < s; i++){
			Type *t = new Type(ts.query(i));
			append(t);
		}
		LEAVE_FUNCTION();
	}

	template <typename Type> bool OrderedSet<Type>::qSortIsLess(const Type* s1, const Type* s2)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = *s1 < *s2;
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	template <typename Type> bool operator==(const OrderedSet<Type> &a, const OrderedSet<Type> &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
	
	template <typename Type> bool operator!=(const OrderedSet<Type> &a, const OrderedSet<Type> &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = !a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
}

#endif

