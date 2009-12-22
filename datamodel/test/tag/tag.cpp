/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerTag>

#include <QtTest/QtTest>

namespace Butler {

	class TestTag : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void construction();
			void equality();
			void comparison();
	};


	void TestTag::construction()
	{
		/* default constructor*/
		{
			Tag tag;

			QCOMPARE(tag.name, QString(""));
			QVERIFY(tag.checked == false);
			QBENCHMARK {
				Tag tag;
			}
		}

		/* parameterized constructor*/
		{
			Tag tag("tag-name");

			QCOMPARE(tag.name, QString("tag-name"));
			QVERIFY(tag.checked == false);
			QBENCHMARK {
				Tag tag("tag-name");
			}
		}

		/* copy constructor*/
		{
			Tag t("tag-name");
			t.checked = true;

			Tag tag(t);

			QCOMPARE(tag.name, t.name);
			QVERIFY(tag.checked == t.checked);
			QBENCHMARK {
				Tag tag(t);
			}
		}
	}

	void TestTag::equality()
	{
		Tag a("a"), b("A");

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.name = "a";
		b.checked = true;

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		a.checked = true;

		QVERIFY(a == b);
		QVERIFY(!(a != b));

		a.checked = false;
		a.name = "b";
		a = b;

		QVERIFY(a.name == b.name);
		QVERIFY(a.checked == b.checked);
	}

	void TestTag::comparison()
	{
		/* dont test comparison itself, just consistency */
		Tag a("a"), b("B");

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
	
QTEST_MAIN(Butler::TestTag);

#include "tmp/tag.moc"

