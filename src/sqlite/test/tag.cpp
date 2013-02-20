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

class TestTag : public QObject
{
public:
	void initTestCase();

	void database();
	void insert();
	void update();

	/* FIXME: Can we change an already referenced tag's name? */
};


void TestTag::initTestCase()
{
	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.create());
}

void TestTag::database()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());

	/* Are checks working on a fresh database? */
	VERIFY(db.check());

	VERIFY(db.close());
}

void TestTag::insert()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());

	/* Lets test insert and query of some tricky name. */
	Tag t("\"name's");
	TagSet ts;

	/* Check if we can insert a new tag. */
	VERIFY(db.tag().insert(t));
	VERIFY(db.tag().query(ts));

	VERIFY(ts.size() == 1);
	VERIFY(t == ts.queryAt(0));

	/* Lets clean up */
	VERIFY(db.tag().del(t));
	VERIFY(db.tag().query(ts));
	VERIFY(ts.size() == 0);

	VERIFY(db.close());
}

void TestTag::update()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());

	/* Lets update an inserted tag. */
	Tag t("orig name");
	Tag tu("updated name");
	TagSet ts;

	/* Check if we can change the name of an existing tag. */
	VERIFY(db.tag().insert(t));
	VERIFY(db.tag().update(t, tu));
	VERIFY(db.tag().query(ts));

	VERIFY(ts.size() == 1);
	VERIFY(tu == ts.queryAt(0));

	/* Lets clean up */
	VERIFY(db.tag().del(tu));
	VERIFY(db.tag().query(ts));
	VERIFY(ts.size() == 0);

	VERIFY(db.close());
}

TEST_INIT(Tag)

	TEST_RUN(database);
	TEST_RUN(insert);
	TEST_RUN(update);

TEST_FINISH(Tag)

