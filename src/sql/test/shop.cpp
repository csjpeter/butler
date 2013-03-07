/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>

#include <csjp_test.h>

#include <butler_db.h>

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

	SqlConnection conn(DB_FILE);
	Db db(conn);

	VERIFY(db.connect());
	VERIFY(db.create());
}

void TestShop::insert()
{
	NOEXC_VERIFY(initTestCase());

	SqlConnection conn(DB_FILE);
	Db db(conn);

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

	SqlConnection conn(DB_FILE);
	Db db(conn);

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

