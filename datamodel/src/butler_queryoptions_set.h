/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYOPTIONS_SET_H
#define BUTLER_QUERYOPTIONS_SET_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>

#include <ButlerQueryOptions>

namespace Butler {

	/* Methods accepting pointers in their parameters will take the
	 * ownership of the pointed object. */
	class QueryOptionsSet : public QObject
	{
		private:
			Q_OBJECT;
		public:
			QueryOptionsSet();
			~QueryOptionsSet();

			explicit QueryOptionsSet(const QueryOptionsSet &tagSet);
			QueryOptionsSet& operator=(const QueryOptionsSet&);

			void append(QueryOptions *qo);
			void remove(int i);
			void clear();
			void move(int from, int to);
			void swap(int i, int j);

			const QueryOptions& query(int i) const;

			bool empty() const;
			int size() const;

			void sort();

			static bool isEqual(const QueryOptionsSet &a, const QueryOptionsSet &b);

		private:
			void equal(const QueryOptionsSet &ts);
			static bool qSortIsLess(
					const QueryOptions* s1, const QueryOptions* s2);

			QList<QueryOptions*> data;
			QMap<const QString*, const QueryOptions*> nameToPtr;
	};

	bool operator==(const QueryOptionsSet &a, const QueryOptionsSet &b);
	bool operator!=(const QueryOptionsSet &a, const QueryOptionsSet &b);

}

#endif

