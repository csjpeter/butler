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

	class DbIfc : public ButlerDb
	{
		DbIfc(){}
		~DbIfc(){}


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
			return new TagSet();
		}
		
		ItemSet* queryItems(const QueryOptions &qo)
		{
			return new ItemSet();
		}

		TagSet* queryTags(const Item &item)
		{
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

		QueryOptions _qo("default");
		Item _i("example");

		TagSet *ts;
		QueryOptionsSet *qos;
		ItemSet *is;


		ts = queryTags();
		Q_VERIFY(ts != NULL);
		delete ts;

		qos = queryQueryOptions("teszt");
		Q_VERIFY(qos != NULL);
		Q_VERIFY(qos->name == "teszt");
		delete qos;

		ts = queryTags(_qo);
		Q_VERIFY(ts != NULL);
		delete ts;

		is = queryItems(_qo);
		Q_VERIFY(is != NULL);
		delete is;

		ts = queryTags(_i);
		Q_VERIFY(ts != NULL);
		delete ts;
	}

}
	
QTEST_MAIN(Butler::TestDbIfc);

#include "tmp/dbifc.moc"

