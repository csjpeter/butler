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
#include <ButlerQuery>

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

			const QString& lastError()
			{
				static QString err("no-error");
				return err;
			}

			/*
			 * Tags
			 */

			bool insertTag(const Tag &t)
			{
				Q_UNUSED(t);
				return true;
			}

			bool updateTag(
				const Tag &orig,
				const Tag &modified)
			{
				Q_UNUSED(orig);
				Q_UNUSED(modified);
				return true;
			}

			bool deleteTag(const Tag &t)
			{
				Q_UNUSED(t);
				return true;
			}

			TagSet* queryTags()
			{
				return new TagSet();
			}

			/*
			 * Query
			 */

			bool insertQuery(const Query &qo)
			{
				Q_UNUSED(qo);
				return true;
			}

			bool updateQuery(
				const Query &orig,
				const Query &modified)
			{
				Q_UNUSED(orig);
				Q_UNUSED(modified);
				return true;
			}

			bool deleteQuery(const Query &qo)
			{
				Q_UNUSED(qo);
				return true;
			}

			Query* queryQuery(const QString &name)
			{
				return new Query(name);
			}

			TagSet* queryTags(const Query &qo)
			{
				Q_UNUSED(qo);

				return new TagSet();
			}

			/*
			 * Items
			 */

			bool insertItem(const Item &i)
			{
				Q_UNUSED(i);
				return true;
			}

			bool updateItem(
				const Item &orig,
				const Item &modified)
			{
				Q_UNUSED(orig);
				Q_UNUSED(modified);
				return true;
			}

			bool deleteItem(const Item &i)
			{
				Q_UNUSED(i);
				return true;
			}

			ItemSet* queryItems(const Query &qo)
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
			
			void db();
			void tags();
			void query();
			void items();
	};


	void TestDbIfc::initTestCase()
	{
	}

	void TestDbIfc::cleanupTestCase()
	{
		_reportLeakSuspections();
	}

	void TestDbIfc::db()
	{
		DbIfc ifc;

		ifc.connect();
		ifc.open();
		ifc.close();
		ifc.lastError();
	}
	
	void TestDbIfc::tags()
	{
		DbIfc ifc;

		Tag a, b;
		ifc.insertTag(a);
		ifc.updateTag(a, b);
		ifc.deleteTag(a);

		TagSet *ts;
		ts = ifc.queryTags();
		QVERIFY(ts != NULL);
		delete ts;
	}
	
	void TestDbIfc::query()
	{
		DbIfc ifc;

		Query a, b;
		ifc.insertQuery(a);
		ifc.updateQuery(a, b);
		ifc.deleteQuery(a);

		Query _qo("default");
		TagSet *ts;
		
		ts = ifc.queryTags(_qo);
		QVERIFY(ts != NULL);
		delete ts;
		
		Query *qo;
		qo = ifc.queryQuery("teszt");
		QVERIFY(qo != NULL);
		QVERIFY(qo->name == "teszt");
		delete qo;
	}
	
	void TestDbIfc::items()
	{
		DbIfc ifc;

		Item a, b;
		ifc.insertItem(a);
		ifc.updateItem(a, b);
		ifc.deleteItem(a);

		TagSet *ts;
		ItemSet *is;

		Query _qo("default");
		is = ifc.queryItems(_qo);
		QVERIFY(is != NULL);
		delete is;

		Item _i("example");
		ts = ifc.queryTags(_i);
		QVERIFY(ts != NULL);
		delete ts;
	}

}
	
QTEST_MAIN(Butler::TestDbIfc);

#include "tmp/dbifc.moc"

