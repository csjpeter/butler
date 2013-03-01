/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>

#include <csjp_test.h>

#include <butler_db.h>

#include <butler_sqlitedb.h>

#define DB_FILE TESTDIR "/TestSqlite.db"

class TestWare : public QObject
{
public:
	void initTestCase();
	
	void insert();
	void update();
	void tags();
};

void TestWare::initTestCase()
{
	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.create());
}

void TestWare::insert()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());
	
	Tag t;
	TagSet ts;
	
	t.name = "meal";
	VERIFY(db.tag().insert(t));

	Ware w;
	WareSet ws;

	w.name = "bread";
	w.unit = "kg";
	w.categories.add(new QString("white"));
	w.categories.add(new QString("brown"));
	w.tags.add(new QString("meal"));

	/* insert and query sould go fine */
	VERIFY(db.ware().insert(w));
	VERIFY(db.ware().query(ws));

	/* lets check if everything went fine */
	VERIFY(ws.size() == 1);
	VERIFY(ws.queryAt(0).name == "bread");
	VERIFY(ws.queryAt(0).unit == "kg");
	VERIFY(ws.queryAt(0).categories.has("white"));
	VERIFY(ws.queryAt(0).categories.has("brown"));
	VERIFY(ws.queryAt(0).categories.size() == 2);
	VERIFY(ws.queryAt(0).tags.size() == 1);
	VERIFY(ws.queryAt(0).tags.has("meal"));
	VERIFY(ws.queryAt(0) == w);

	/* Lets clean up */
	VERIFY(db.ware().del(w));
	VERIFY(db.ware().query(ws));
	VERIFY(ws.size() == 0);
	
	VERIFY(db.tag().del(t));
	VERIFY(db.tag().query(ts));
	VERIFY(ts.size() == 0);

	VERIFY(db.close());
}

void TestWare::update()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());
	
	Tag meal("meal");
	VERIFY(db.tag().insert(meal));
	Tag drink("drink");
	VERIFY(db.tag().insert(drink));
	TagSet ts;

	/* Lets update an inserted ware. */
	Ware w("bread");
	w.unit = "kg";
	w.categories.add(new QString("white"));
	w.tags.add(new QString("meal"));
	Ware wu("milk");
	wu.unit = "l";
	wu.categories.add(new QString("fat"));
	wu.tags.add(new QString("meal"));
	wu.tags.add(new QString("drink"));

	WareSet ws;
	
	VERIFY(db.ware().insert(w));

	/* Update should be ok. */
	VERIFY(db.ware().update(w, wu));
	VERIFY(db.ware().query(ws));
	VERIFY(ws.size() == 1);
	VERIFY(wu == ws.queryAt(0));
	VERIFY(ws.queryAt(0).name == "milk");
	VERIFY(ws.queryAt(0).unit == "l");
	VERIFY(ws.queryAt(0).categories.size() == 1);
	VERIFY(ws.queryAt(0).categories.has("fat"));
	VERIFY(ws.queryAt(0).tags.size() == 2);
	VERIFY(ws.queryAt(0).tags.has("meal"));
	VERIFY(ws.queryAt(0).tags.has("drink"));

	/* Lets clean up */
	VERIFY(db.ware().del(wu));
	VERIFY(db.ware().query(ws));
	VERIFY(ws.size() == 0);
	
	VERIFY(db.tag().del(meal));
	VERIFY(db.tag().del(drink));
	VERIFY(db.tag().query(ts));
	VERIFY(ts.size() == 0);

	VERIFY(db.close());
}

void TestWare::tags()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());

	Ware w("bread");
	w.tags.add(new QString("meal"));

	WareSet ws;
	
	/* Insert with unregistered tag should fail. */
	VERIFY(!db.ware().insert(w));

	/* Insert without tag should be ok. */
	w.tags.clear();
	VERIFY(db.ware().insert(w));

	/* Lets clean up */
	VERIFY(db.ware().del(w));
	VERIFY(db.ware().query(ws));
	VERIFY(ws.size() == 0);

	VERIFY(db.close());
}

TEST_INIT(Ware)

	TEST_RUN(insert);
	TEST_RUN(update);
	TEST_RUN(tags);

TEST_FINISH(Ware)

