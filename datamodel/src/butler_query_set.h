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
#include <QMap>

#include <ButlerQuery>
#include <ButlerOrderedSet>

namespace Butler {

	/* Methods accepting pointers in their parameters will take the
	 * ownership of the pointed object. */
	class QuerySet : public QObject, public OrderedSet<Query>
	{
		private:
			Q_OBJECT;
		public:
			QuerySet();
			~QuerySet();

			explicit QuerySet(const QuerySet &);
			QuerySet& operator=(const QuerySet &);

			void append(Query *);
			void remove(int i);
			void clear();

		private:
			QMap<const QString*, const Query*> nameToPtr;
	};

}

#endif

