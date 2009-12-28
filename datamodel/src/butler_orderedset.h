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

	template <typename Type> class OrderedSet
	{
		public:
			OrderedSet()
			{
				ENTER_CONSTRUCTOR();
				LEAVE_CONSTRUCTOR();
			}

			virtual ~OrderedSet()
			{
				ENTER_DESTRUCTOR();
				clear();
				LEAVE_DESTRUCTOR();
			}

			explicit OrderedSet(const OrderedSet &os)
			{
				ENTER_CONSTRUCTOR();
				equal(os);
				LEAVE_CONSTRUCTOR();
			}

			virtual OrderedSet& operator=(const OrderedSet &os)
			{
				ENTER_FUNCTION();
				clear();
				equal(os);
				LEAVE_FUNCTION();
				return *this;
			}

			virtual void append(Type *t)
			{
				ENTER_FUNCTION();
				Q_ASSERT(t);
				data.append(t);
				LEAVE_FUNCTION();
			}

			virtual void remove(int i)
			{
				ENTER_FUNCTION();
				Q_ASSERT(i < data.size());
				Q_ASSERT(0 <= i);
				Type *t = data.at(i);
				data.removeAt(i);
				delete t;
				LEAVE_FUNCTION();
			}

			virtual void clear()
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

			virtual void move(int from, int to)
			{
				ENTER_FUNCTION();
				Q_ASSERT(from < data.size());
				Q_ASSERT(0 <= from);
				Q_ASSERT(to < data.size());
				Q_ASSERT(0 <= to);
				data.move(from, to);
				LEAVE_FUNCTION();
			}

			const Type& query(int i) const
			{
				ENTER_FUNCTION();
				Q_ASSERT(i<data.size());
				Type *t = data.at(i);
				Q_ASSERT(t != NULL);
				LEAVE_FUNCTION();
				return *t;
			}

			bool empty() const
			{
				ENTER_FUNCTION();
				bool ret = data.isEmpty();
				LEAVE_FUNCTION();
				return ret;
			}

			int size() const
			{
				ENTER_FUNCTION();
				int ret = data.size();
				LEAVE_FUNCTION();
				return ret;
			}

			void sort()
			{
				ENTER_FUNCTION();
				qSort(data.begin(), data.end(), OrderedSet::qSortIsLess);
				LEAVE_FUNCTION();
			}

			bool isEqual(const OrderedSet &os) const
			{
				ENTER_STATIC_FUNCTION();
				bool ret = true;
				int s = size();

				if(s != os.size())
					ret = false;
				else {
					int i;
					for(i=0; i < s; i++){
						const Type &ta = query(i);
						const Type &tb = os.query(i);
						if(ta != tb){
							ret = false;
							break;
						}
					}
				}

				LEAVE_STATIC_FUNCTION();
				return ret;
			}

		protected:
			void equal(const OrderedSet &os)
			{
				ENTER_FUNCTION();
				int s = os.size();
				int i;
				for(i=0; i < s; i++){
					Type *t = new Type(os.query(i));
					append(t);
				}
				LEAVE_FUNCTION();
			}

		private:
			static bool qSortIsLess(
					const Type* s1, const Type* s2)
			{
				ENTER_STATIC_FUNCTION();
				bool ret = *s1 < *s2;
				LEAVE_STATIC_FUNCTION();
				return ret;
			}

			QList<Type*> data;
	};


	template <typename Type> bool operator==(
			const OrderedSet<Type> &a, const OrderedSet<Type> &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}

	template <typename Type> bool operator!=(
			const OrderedSet<Type> &a, const OrderedSet<Type> &b)
	{
		ENTER_STATIC_FUNCTION();
		bool ret = !a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
		return ret;
	}
}

#endif

