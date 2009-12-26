/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerQueryOptionsSet>

#include <QtTest/QtTest>

namespace Butler {

	class TestQueryOptionsSet : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void construction();
			void insertion();
			void removation();
			void copy();
			void ordering();
			void equality();
	};


	void TestQueryOptionsSet::construction()
	{
		QueryOptionsSet qos;

		QVERIFY(qos.size() == 0);
		QVERIFY(qos.empty() == true);
	}
	
	void TestQueryOptionsSet::insertion()
	{
		QueryOptions *qo = new QueryOptions("first");
		QueryOptionsSet qos;
		qos.append(qo);

		QVERIFY(qos.size() == 1);

		const QueryOptions &q = qos.query(0);

		QVERIFY(q.name == "first");
	}

	void TestQueryOptionsSet::removation()
	{
		QueryOptionsSet qos;
		qos.append(new QueryOptions("first"));
		qos.append(new QueryOptions("second"));
		qos.append(new QueryOptions("third"));

		qos.remove(1);

		QVERIFY(qos.size() == 2);
		QVERIFY(qos.query(0).name == "first");
		QVERIFY(qos.query(1).name == "third");
	}

	void TestQueryOptionsSet::copy()
	{
		QueryOptions *qo = new QueryOptions("first");
		QueryOptionsSet qos;
		qos.append(qo);

		{
			QueryOptionsSet qos_copy(qos);

			QCOMPARE(qos_copy.size(), 1);
			QVERIFY(qos_copy.empty() == false);
			QVERIFY(qos.query(0) == qos_copy.query(0));
		}

		{
			QueryOptionsSet qos_copy;
			qos_copy.append(new QueryOptions("old"));
			qos_copy = qos;

			QCOMPARE(qos_copy.size(), 1);
			QVERIFY(qos_copy.empty() == false);
			QVERIFY(qos.query(0) == qos_copy.query(0));
		}
	}

	void TestQueryOptionsSet::ordering()
	{
		{
			QueryOptionsSet qos;
			qos.append(new QueryOptions("xyz"));
			qos.append(new QueryOptions("uvw"));
			qos.append(new QueryOptions("rst"));
			qos.sort();
			QVERIFY(qos.query(0).name == "rst");
			QVERIFY(qos.query(1).name == "uvw");
			QVERIFY(qos.query(2).name == "xyz");
		}

		{
			QueryOptionsSet qos;
			qos.append(new QueryOptions("xyz"));
			qos.append(new QueryOptions("uvw"));
			qos.append(new QueryOptions("rst"));
			qos.move(2, 0);
			QVERIFY(qos.query(0).name == "rst");
			QVERIFY(qos.query(1).name == "xyz");
			QVERIFY(qos.query(2).name == "uvw");
		}

		{
			QueryOptionsSet qos;
			qos.append(new QueryOptions("xyz"));
			qos.append(new QueryOptions("uvw"));
			qos.append(new QueryOptions("rst"));
			qos.swap(0,2);
			QVERIFY(qos.query(0).name == "rst");
			QVERIFY(qos.query(1).name == "uvw");
			QVERIFY(qos.query(2).name == "xyz");
		}
	}

	void TestQueryOptionsSet::equality()
	{
		QueryOptionsSet a, b;
		
		a.append(new QueryOptions("xyz"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.append(new QueryOptions("xy"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.clear();
		b.append(new QueryOptions("xyz"));

		QVERIFY(a == b);
		QVERIFY(!(a != b));

		QVERIFY(a.query(0) == b.query(0));
		QVERIFY(a.size() == b.size());
	}

}
	
QTEST_MAIN(Butler::TestQueryOptionsSet);

#include "tmp/queryoptions_set.moc"

