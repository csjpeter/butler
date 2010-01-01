/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtTest/QtTest>

#include <ButlerDebug>
#include <ButlerDb>
#include <ButlerItemSet>
#include <ButlerTagSet>
#include <ButlerQueryOptions>

namespace Butler {

	class DbIfc : public Db
	{
		public:
			DbIfc(){}
			~DbIfc(){}

			bool connect()
			{
				return true;
			}

			bool open()
			{
				return true;
			}

			bool close()
			{
				return true;
			}

			TagSet* queryTags()
			{
				return new TagSet();
			}

			QueryOptions* queryQueryOptions(const QString &name)
			{
				return new QueryOptions(name);
			}

			TagSet* queryTags(const QueryOptions &qo)
			{
				Q_UNUSED(qo);

				return new TagSet();
			}

			ItemSet* queryItems(const QueryOptions &qo)
			{
				Q_UNUSED(qo);

				return new ItemSet();
			}

			TagSet* queryTags(const Item &item)
			{
				Q_UNUSED(item);

				return new TagSet();
			}
	};

	class TestDbIfc : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void initTestCase();
			void cleanupTestCase();
			
			void methodcalls();
	};


	void TestDbIfc::initTestCase()
	{
	}

	void TestDbIfc::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestDbIfc::methodcalls()
	{
		DbIfc ifc;

		ifc.connect();
		ifc.open();
		ifc.close();

		QueryOptions _qo("default");
		Item _i("example");

		TagSet *ts;
		QueryOptions *qo;
		ItemSet *is;


		ts = ifc.queryTags();
		QVERIFY(ts != NULL);
		delete ts;

		qo = ifc.queryQueryOptions("teszt");
		QVERIFY(qo != NULL);
		QVERIFY(qo->name == "teszt");
		delete qo;

		ts = ifc.queryTags(_qo);
		QVERIFY(ts != NULL);
		delete ts;

		is = ifc.queryItems(_qo);
		QVERIFY(is != NULL);
		delete is;

		ts = ifc.queryTags(_i);
		QVERIFY(ts != NULL);
		delete ts;
	}

}
	
QTEST_MAIN(Butler::TestDbIfc);

#include "tmp/dbifc.moc"

