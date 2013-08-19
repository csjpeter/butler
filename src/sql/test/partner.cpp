/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>

#include <csjp_test.h>

#include <butler_db.h>

#define DB_FILE TESTDIR "/TestSqlite.db"

DatabaseDescriptor sqliteDesc;

class TestPartner : public QObject
{
public:
	void initTestCase();
	
	void insert();
	void update();
};


void TestPartner::initTestCase()
{
	sqliteDesc.name = "localdb";
	sqliteDesc.driver = "QSQLITE";
	sqliteDesc.databaseName = DB_FILE;

	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	Db db(sqliteDesc);

}

void TestPartner::insert()
{
	NOEXC_VERIFY(initTestCase());

	Db db(sqliteDesc);

	VERIFY(db.open());

	Partner s;
	PartnerSet ss;

	s.name = "western tesco";
	s.storeName = "number xy";
	s.city = "Pecs";
	s.address = "postal code, street and number";
	s.company = "tesco";

	/* put a partner in db and read it back */
	VERIFY(db.partner.insert(s));
	VERIFY(db.partner.query(ss));
	VERIFY(ss.size() == 1);
	VERIFY(ss.queryAt(0) == s);

	/* Lets clean up */
	VERIFY(db.partner.del(s));
	VERIFY(db.partner.query(ss));
	VERIFY(ss.size() == 0);

}

void TestPartner::update()
{
	NOEXC_VERIFY(initTestCase());

	Db db(sqliteDesc);

	VERIFY(db.open());


	Partner s, su;
	PartnerSet ss;

	VERIFY(db.partner.insert(s));

	/* These fields must be updated */
	su.name = "western tesco";
	su.storeName = "number xy";
	su.city = "Pecs";
	su.address = "postal code, street and number";
	su.company = "tesco";

	VERIFY(db.partner.update(s, su));

	VERIFY(db.partner.query(ss));
	VERIFY(ss.size() == 1);
	VERIFY(su == ss.queryAt(0));

	/* Lets clean up */
	VERIFY(db.partner.del(su));
	VERIFY(db.partner.query(ss));
	VERIFY(ss.size() == 0);

}

TEST_INIT(Partner)

	TEST_RUN(insert);
	TEST_RUN(update);

TEST_FINISH(Partner)

