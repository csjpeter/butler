/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerTagSet>

#include <butler_debug.h>

#include <QtTest/QtTest>

namespace Butler {

	class TestTagSet : public QObject
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

	void TestTagSet::initTestCase()
	{
	}

	void TestTagSet::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestTagSet::construction()
	{
		TagSet ts;

		QCOMPARE(ts.size(), 0);
		QVERIFY(ts.empty() == true);
	}
	
	void TestTagSet::insertion()
	{
		Tag *t = new Tag("first");
		TagSet ts;
		ts.append(t);

		QVERIFY(ts.size() == 1);

		const Tag &q = ts.query(0);

		QVERIFY(q.name == "first");
	}

	void TestTagSet::removation()
	{
		TagSet ts;
		ts.append(new Tag("first"));
		ts.append(new Tag("second"));
		ts.append(new Tag("third"));

		ts.remove(1);

		QVERIFY(ts.size() == 2);
		QVERIFY(ts.query(0).name == "first");
		QVERIFY(ts.query(1).name == "third");
	}

	void TestTagSet::copy()
	{
		Tag *t = new Tag("first");
		TagSet ts;
		ts.append(t);

		{
			TagSet ts_copy(ts);

			QCOMPARE(ts_copy.size(), 1);
			QVERIFY(ts_copy.empty() == false);
			QVERIFY(ts.query(0) == ts_copy.query(0));
		}

		{
			TagSet ts_copy;
			ts_copy.append(new Tag("old"));
			ts_copy = ts;

			QCOMPARE(ts_copy.size(), 1);
			QVERIFY(ts_copy.empty() == false);
			QVERIFY(ts.query(0) == ts_copy.query(0));
		}
	}

}
	
QTEST_MAIN(Butler::TestTagSet);

#include "tmp/tag_set.moc"

