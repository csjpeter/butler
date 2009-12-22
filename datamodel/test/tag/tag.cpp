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
			void defaultConstructor();
	};


	void TestTag::defaultConstructor()
	{
		Tag tag;

		QCOMPARE(tag.name, QString(""));
		QVERIFY(tag.checked == false);
		QBENCHMARK {
			Tag tag;
		}
	}

}
	
QTEST_MAIN(Butler::TestTag);

#include "tmp/tag.moc"

