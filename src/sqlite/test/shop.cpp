/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>

#include <csjp_test.h>
#include <csjp_debug.h>

#include <butler_db.h>

#include <butler_sqlitedb.h>

#define DB_FILE TESTDIR "/TestSqlite.db"

class TestShop : public QObject
{
public:
	void initTestCase();
	
	void insert();
	void update();
};


void TestShop::initTestCase()
{
	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.create());
}

void TestShop::insert()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());

	Shop s;
	ShopSet ss;

	s.name = "western tesco";
	s.storeName = "number xy";
	s.city = "Pecs";
	s.address = "postal code, street and number";
	s.company = "tesco";

	/* put a shop in db and read it back */
	VERIFY(db.shop().insert(s));
	VERIFY(db.shop().query(ss));
	VERIFY(ss.size() == 1);
	VERIFY(ss.queryAt(0) == s);

	/* Lets clean up */
	VERIFY(db.shop().del(s));
	VERIFY(db.shop().query(ss));
	VERIFY(ss.size() == 0);

	VERIFY(db.close());
}

void TestShop::update()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());


	Shop s, su;
	ShopSet ss;

	VERIFY(db.shop().insert(s));

	/* These fields must be updated */
	su.name = "western tesco";
	su.storeName = "number xy";
	su.city = "Pecs";
	su.address = "postal code, street and number";
	su.company = "tesco";

	VERIFY(db.shop().update(s, su));

	VERIFY(db.shop().query(ss));
	VERIFY(ss.size() == 1);
	VERIFY(su == ss.queryAt(0));

	/* Lets clean up */
	VERIFY(db.shop().del(su));
	VERIFY(db.shop().query(ss));
	VERIFY(ss.size() == 0);

	VERIFY(db.close());
}

TEST_INIT(Shop)

	TEST_RUN(insert);
	TEST_RUN(update);

TEST_FINISH(Shop)

