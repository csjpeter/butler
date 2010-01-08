/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerQuerySet>

#include <butler_debug.h>

#include <QtTest/QtTest>

namespace Butler {

	class TestQuerySet : public QObject
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


	void TestQuerySet::initTestCase()
	{
	}

	void TestQuerySet::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestQuerySet::construction()
	{
		QuerySet qos;

		QVERIFY(qos.size() == 0);
		QVERIFY(qos.empty() == true);
	}
	
	void TestQuerySet::insertion()
	{
		Query *qo = new Query("first");
		QuerySet qos;
		qos.append(qo);

		QVERIFY(qos.size() == 1);

		const Query &q = qos.query(0);

		QVERIFY(q.name == "first");
	}

	void TestQuerySet::removation()
	{
		QuerySet qos;
		qos.append(new Query("first"));
		qos.append(new Query("second"));
		qos.append(new Query("third"));

		qos.remove(1);

		QVERIFY(qos.size() == 2);
		QVERIFY(qos.query(0).name == "first");
		QVERIFY(qos.query(1).name == "third");
	}

	void TestQuerySet::copy()
	{
		Query *qo = new Query("first");
		QuerySet qos;
		qos.append(qo);

		{
			QuerySet qos_copy(qos);

			QCOMPARE(qos_copy.size(), 1);
			QVERIFY(qos_copy.empty() == false);
			QVERIFY(qos.query(0) == qos_copy.query(0));
		}

		{
			QuerySet qos_copy;
			qos_copy.append(new Query("old"));
			qos_copy = qos;

			QCOMPARE(qos_copy.size(), 1);
			QVERIFY(qos_copy.empty() == false);
			QVERIFY(qos.query(0) == qos_copy.query(0));
		}
	}

}
	
QTEST_MAIN(Butler::TestQuerySet);

#include "tmp/query_set.moc"

