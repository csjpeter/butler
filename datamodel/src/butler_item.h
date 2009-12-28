/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_H
#define BUTLER_ITEM_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QList>

#include <ButlerTagSet>

namespace Butler {

	class Tag;

	class Item : public QObject
	{
		public:
			Q_OBJECT;
		public:
			Item();
			explicit Item(const QString &_name);
			explicit Item(const Item &item);
			~Item();

			Item& operator=(const Item& i);

			bool isEqual(const Item &i) const;

			bool isLess(const Item &i) const;
			bool isMore(const Item &i) const;

		public:
			QString name;
			QDate uploaded;
			int expectedPrice;
			QDate purchased;
			int paidPrice;
			unsigned amortYears;
			unsigned amortMonths;
			unsigned amortDays;
			QString comment;

			TagSet tags;

		private:
			void equal(const Item &i);
	};

	bool operator==(const Item &a, const Item &b);
	bool operator!=(const Item &a, const Item &b);
	bool operator<(const Item &a, const Item &b);
	bool operator>(const Item &a, const Item &b);

}

#endif

