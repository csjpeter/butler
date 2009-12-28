/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QString>
#include <QtTest/QtTest>

#include <ButlerOrderedSet>
#include <ButlerDebug>


namespace Butler {

	class TestOrderedSet : public QObject
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
			void ordering();
			void equality();
	};

	void TestOrderedSet::initTestCase()
	{
	}

	void TestOrderedSet::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestOrderedSet::construction()
	{
		OrderedSet<QString> ts;

		QCOMPARE(ts.size(), 0);
		QVERIFY(ts.empty() == true);
	}
	
	void TestOrderedSet::insertion()
	{
		QString *s = new QString("first");
		OrderedSet<QString> ts;
		ts.append(s);

		QVERIFY(ts.size() == 1);

		const QString &q = ts.query(0);

		QVERIFY(q == "first");
	}

	void TestOrderedSet::removation()
	{
		OrderedSet<QString> ts;
		ts.append(new QString("first"));
		ts.append(new QString("second"));
		ts.append(new QString("third"));

		ts.remove(1);

		QVERIFY(ts.size() == 2);
		QVERIFY(ts.query(0) == "first");
		QVERIFY(ts.query(1) == "third");
	}

	void TestOrderedSet::copy()
	{
		QString *t = new QString("first");
		OrderedSet<QString> ts;
		ts.append(t);

		{
			OrderedSet<QString> ts_copy(ts);

			QCOMPARE(ts_copy.size(), 1);
			QVERIFY(ts_copy.empty() == false);
			QVERIFY(ts.query(0) == ts_copy.query(0));
		}

		{
			OrderedSet<QString> ts_copy;
			ts_copy.append(new QString("old"));
			ts_copy = ts;

			QCOMPARE(ts_copy.size(), 1);
			QVERIFY(ts_copy.empty() == false);
			QVERIFY(ts.query(0) == ts_copy.query(0));
		}
	}

	void TestOrderedSet::ordering()
	{
		{
			OrderedSet<QString> ts;
			ts.append(new QString("xyz"));
			ts.append(new QString("uvw"));
			ts.append(new QString("rst"));
			ts.sort();
			QVERIFY(ts.query(0) == "rst");
			QVERIFY(ts.query(1) == "uvw");
			QVERIFY(ts.query(2) == "xyz");
		}

		{
			OrderedSet<QString> ts;
			ts.append(new QString("xyz"));
			ts.append(new QString("uvw"));
			ts.append(new QString("rst"));
			ts.move(2, 0);
			QVERIFY(ts.query(0) == "rst");
			QVERIFY(ts.query(1) == "xyz");
			QVERIFY(ts.query(2) == "uvw");
		}
	}

	void TestOrderedSet::equality()
	{
		OrderedSet<QString> a, b;
		
		a.append(new QString("xyz"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.append(new QString("xy"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.clear();
		b.append(new QString("xyz"));

		QVERIFY(a == b);
		QVERIFY(!(a != b));

		QVERIFY(a.query(0) == b.query(0));
		QVERIFY(a.size() == b.size());
	}

}
	
QTEST_MAIN(Butler::TestOrderedSet);

#include "tmp/orderedset.moc"

