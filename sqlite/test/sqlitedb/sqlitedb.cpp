/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>
#include <QtTest/QtTest>

#include <ButlerDebug>
#include <ButlerSqliteDb>

#define DB_FILE "./TestSqliteDb.db"

namespace Butler {

	class TestSqliteDb : public QObject
	{
		private:
			Q_OBJECT;
		private slots:
			void initTestCase();
			void cleanupTestCase();
			
			void connection();
	};


	void TestSqliteDb::initTestCase()
	{
	}

	void TestSqliteDb::cleanupTestCase()
	{
		QFile f(DB_FILE);
		QVERIFY(f.remove());

		_reportLeakSuspections();
	}

	void TestSqliteDb::connection()
	{
		{
			Db *sql = new SqliteDb(DB_FILE);

			QVERIFY(sql->connect());

			delete sql;
		}
		{
			Db *sql = new SqliteDb(DB_FILE);

			QVERIFY(sql->connect());
			QVERIFY(sql->open());
			QVERIFY(sql->close());
			QVERIFY(sql->open());
			
			delete sql;
		}

	}

}
	
QTEST_MAIN(Butler::TestSqliteDb);

#include "tmp/sqlitedb.moc"
