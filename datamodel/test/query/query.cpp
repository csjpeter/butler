/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtTest/QtTest>

#include <ButlerDebug>
#include <ButlerQuery>

namespace Butler {

	class TestQuery : public QObject
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


	void TestQuery::initTestCase()
	{
	}

	void TestQuery::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestQuery::construction()
	{
		/* default constructor*/
		{
			Query qo;

			QCOMPARE(qo.name, QString(""));
			QVERIFY(qo.tags.size() == 0);
		}

		/* parameterized constructor*/
		{
			Query qo("default");

			QCOMPARE(qo.name, QString("default"));
			QVERIFY(qo.tags.size() == 0);
		}

		/* copy constructor*/
		{
			Query qo("default");
			qo.tags.append(new Tag("tag-name"));
			qo.startDate = QDate(2000, 1, 1);
			qo.endDate = QDate(2000, 11, 11);

			Query qo_copy(qo);

			QVERIFY(qo_copy.name == "default");
			QVERIFY(qo_copy.tags.size() == 1);
			QVERIFY(qo_copy.startDate == QDate(2000, 1, 1));
			QVERIFY(qo_copy.endDate == QDate(2000, 11, 11));
		}
	}

	void TestQuery::equality()
	{
		Query a, b;
		
		b.name = "b";
		b.tags.append(new Tag("tag-name"));
		b.startDate = QDate(2000, 1, 1);
		b.endDate = QDate(2000, 11, 11);
		a = b;
		
		QVERIFY(a.name == b.name);
		QVERIFY(a.tags.size() == b.tags.size());
		QVERIFY(a.startDate == b.startDate);
		QVERIFY(a.endDate == b.endDate);
		QVERIFY(a == b);
		QVERIFY(!(a != b));

		b.name = "B";

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b = a;
		b.tags.append(new Tag("tag-name2"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b = a;
		b.startDate = QDate(2000, 1, 2);

		QVERIFY(!(a == b));
		QVERIFY(a != b);
		
		b = a;
		b.endDate = QDate(2000, 1, 2);

		QVERIFY(!(a == b));
		QVERIFY(a != b);
	}

	void TestQuery::comparison()
	{
		/* dont test comparison itself, just consistency */
		Query a("a"), b("B");

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
	
QTEST_MAIN(Butler::TestQuery);

#include "tmp/query.moc"

