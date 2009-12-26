/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerQueryOptions>

#include <butler_debug.h>

#include <QtTest/QtTest>

namespace Butler {

	class TestQueryOptions : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void initTestCase();
			void cleanupTestCase();
			
			void construction();
			void equality();
			void comparison();
	};


	void TestQueryOptions::initTestCase()
	{
	}

	void TestQueryOptions::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestQueryOptions::construction()
	{
		/* default constructor*/
		{
			QueryOptions qo;

			QCOMPARE(qo.name, QString(""));
			QVERIFY(qo.tags.size() == 0);
		}

		/* parameterized constructor*/
		{
			QueryOptions qo("default");

			QCOMPARE(qo.name, QString("default"));
			QVERIFY(qo.tags.size() == 0);
		}

		/* copy constructor*/
		{
			QueryOptions qo("default");
			qo.tags.append(new Tag("tag-name"));

			QueryOptions qo_copy(qo);

			QVERIFY(qo_copy.name == "default");
			QVERIFY(qo_copy.tags.size() == 1);
		}
	}

	void TestQueryOptions::equality()
	{
		QueryOptions a("a"), b("A");

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.name = "a";
		b.tags.append(new Tag("tag-name"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		a.tags.append(new Tag("tag-name"));

		QVERIFY(a == b);
		QVERIFY(!(a != b));

		b.name = "b";
		a.tags.clear();
		a = b;

		QVERIFY(a.name == b.name);
		QVERIFY(a.tags.size() == b.tags.size());
	}

	void TestQueryOptions::comparison()
	{
		/* dont test comparison itself, just consistency */
		QueryOptions a("a"), b("B");

		/* one of < and > must be true */
		QVERIFY(a < b || a > b);
		/* result of < and > must be different */
		QVERIFY(!(a < b && a > b));

		a.name = "az3d";
		b.name = "az3d";

		QVERIFY(!(a < b || a > b));

		a.name = "óa";
		b.name = "oB";

		QVERIFY(a < b || a > b);
		QVERIFY(!(a < b && a > b));
	}

}
	
QTEST_MAIN(Butler::TestQueryOptions);

#include "tmp/queryoptions.moc"

