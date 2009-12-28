/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYOPTIONS_H
#define BUTLER_QUERYOPTIONS_H

#include <QObject>
#include <QString>

#include <ButlerTagSet>

namespace Butler {

	class QueryOptions : public QObject
	{
		private:
			Q_OBJECT;
		public:
			QueryOptions();
			explicit QueryOptions(const QString &_name);
			explicit QueryOptions(const QueryOptions &qo);
			~QueryOptions();

			QueryOptions& operator=(const QueryOptions& qo);

			bool isEqual(const QueryOptions &qo) const;

			bool isLess(const QueryOptions &qo) const;
			bool isMore(const QueryOptions &qo) const;

		public:
			QString name;
			TagSet tags;

		private:
			void equal(const QueryOptions &qo);
	};

	bool operator==(const QueryOptions &a, const QueryOptions &b);
	bool operator!=(const QueryOptions &a, const QueryOptions &b);

	bool operator<(const QueryOptions &a, const QueryOptions &b);
	bool operator>(const QueryOptions &a, const QueryOptions &b);
}

#endif

