/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <ButlerItem>

#include <ButlerDebug>

#include <QtTest/QtTest>

namespace Butler {

	class TestItem : public QObject
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


	void TestItem::initTestCase()
	{
	}

	void TestItem::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestItem::construction()
	{
		/* default constructor*/
		{
			Item i;

			QCOMPARE(i.name, QString(""));
			QVERIFY(i.tags.size() == 0);
		}

		/* parameterized constructor*/
		{
			Item i("default");

			QCOMPARE(i.name, QString("default"));
			QVERIFY(i.tags.size() == 0);
		}

		/* copy constructor*/
		{
			Item i;
			i.name = "default";
			i.tags.append(new Tag("tag-name"));

			Item i_copy(i);

			QVERIFY(i_copy.name == "default");
			QVERIFY(i_copy.tags.size() == 1);
		}
	}

	void TestItem::equality()
	{
		Item a, b;
		a.uploaded.setDate(2009, 12, 10);
		b.uploaded.setDate(2010, 12, 10);

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		b.uploaded.setDate(2009, 12, 10);
		b.tags.append(new Tag("tag-name"));

		QVERIFY(!(a == b));
		QVERIFY(a != b);

		a.tags.append(new Tag("tag-name"));

		QVERIFY(a == b);
		QVERIFY(!(a != b));

		b.uploaded.setDate(2008, 12, 10);
		a.tags.clear();
		a = b;

		QVERIFY(a.name == b.name);
		QVERIFY(a.tags.size() == b.tags.size());
	}

	void TestItem::comparison()
	{
		/* dont test comparison itself, just consistency */
		Item a, b;

		a.uploaded.setDate(2009, 12, 10);
		b.uploaded.setDate(2009, 12, 11);

		/* one of < and > must be true */
		QVERIFY(a < b || a > b);
		/* result of < and > must be different */
		QVERIFY(!(a < b && a > b));

		a.uploaded.setDate(2000, 1, 1);
		b.uploaded.setDate(2000, 1, 1);

		QVERIFY(!(a < b || a > b));

		a.uploaded.setDate(2008, 10, 10);
		b.uploaded.setDate(2008, 10, 11);

		QVERIFY(a < b || a > b);
		QVERIFY(!(a < b && a > b));
	}

}
	
QTEST_MAIN(Butler::TestItem);

#include "tmp/item.moc"

