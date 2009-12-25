/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerTagSet>

#include <QtTest/QtTest>

namespace Butler {

	class TestTagSet : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void construction();
			void insertion();
			void copy();
			void removation();
			void ordering();
			void equality();
	};


	void TestTagSet::construction()
	{
		TagSet ts;

		QCOMPARE(ts.size(), 0);
		QVERIFY(ts.empty() == true);

		QBENCHMARK {
			TagSet ts;
		}
	}
	
	void TestTagSet::insertion()
	{
		Tag *t = new Tag("first");
		TagSet ts;
		ts.append(t);

		QVERIFY(ts.size() == 1);

		const Tag &q = ts.query(0);

		QVERIFY(q.name == "first");

		QBENCHMARK {
			ts.append(new Tag("more"));
		}
	}

	void TestTagSet::copy()
	{
		Tag *t = new Tag("first");
		TagSet ts;
		ts.append(t);

		TagSet ts_copy(ts);

		QCOMPARE(ts_copy.size(), 1);
		QVERIFY(ts_copy.empty() == false);
		QVERIFY(ts.query(0) == ts_copy.query(0));

		QBENCHMARK {
			TagSet ts_copy(ts);
		}
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

	void TestTagSet::ordering()
	{
		TagSet ts;
		ts.append(new Tag("xyz"));
		ts.append(new Tag("uvw"));
		ts.append(new Tag("rst"));
		ts.sort();
		QVERIFY(ts.query(0).name == "rst");
		QVERIFY(ts.query(1).name == "uvw");
		QVERIFY(ts.query(2).name == "xyz");
	}

	void TestTagSet::equality()
	{
		TagSet a, b;
		
		a.append(new Tag("xyz"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);
		
		b.append(new Tag("xyz"));

		QVERIFY(a == b);
		QVERIFY(!(a != b));

		QVERIFY(a.query(0) == b.query(0));
		QVERIFY(a.size() == b.size());
	}

}
	
QTEST_MAIN(Butler::TestTagSet);

#include "tmp/tag_set.moc"

