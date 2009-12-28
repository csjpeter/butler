/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtTest/QtTest>

#include <ButlerDebug>
#include <ButlerItemSet>

namespace Butler {

	class TestItemSet : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void initTestCase();
			void cleanupTestCase();
			
			void construction();
			void insertion();
			void removation();
			void copy();
	};


	void TestItemSet::initTestCase()
	{
	}

	void TestItemSet::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestItemSet::construction()
	{
		ItemSet is;

		QVERIFY(is.size() == 0);
		QVERIFY(is.empty() == true);
	}
	
	void TestItemSet::insertion()
	{
		Item *i = new Item("first");
		ItemSet is;
		is.append(i);

		QVERIFY(is.size() == 1);

		const Item &q = is.query(0);

		QVERIFY(q.name == "first");
	}

	void TestItemSet::removation()
	{
		ItemSet is;
		is.append(new Item("first"));
		is.append(new Item("second"));
		is.append(new Item("third"));

		is.remove(1);

		QVERIFY(is.size() == 2);
		QVERIFY(is.query(0).name == "first");
		QVERIFY(is.query(1).name == "third");
	}

	void TestItemSet::copy()
	{
		Item *i = new Item("first");
		ItemSet is;
		is.append(i);

		{
			ItemSet is_copy(is);

			QCOMPARE(is_copy.size(), 1);
			QVERIFY(is_copy.empty() == false);
			QVERIFY(is.query(0) == is_copy.query(0));
		}

		{
			ItemSet is_copy;
			is_copy.append(new Item("old"));
			is_copy = is;

			QCOMPARE(is_copy.size(), 1);
			QVERIFY(is_copy.empty() == false);
			QVERIFY(is.query(0) == is_copy.query(0));
		}
	}

}
	
QTEST_MAIN(Butler::TestItemSet);

#include "tmp/item_set.moc"

