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

class TestItem
{
public:
	void initTestCase();
	
	void insert();
	void update();
	void tagqueries();
	void itemNames();
};


void TestItem::initTestCase()
{
	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.create());
}

void TestItem::insert()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;
	QueryStat stat;

	VERIFY(db.connect());
	VERIFY(db.open());

	Query q("testing query");
//	q.startDate = QDateTime(QDate(2000, 1, 1));
//	q.endDate = QDateTime(QDate::currentDate().addDays(1));
	
	/* we will need shop for inserting bought item */
	ShopSet ss;
	Shop s("myshop");
	VERIFY(db.shop().insert(s));

	Item i("test item");
	ItemSet is;
	TagNameSet qtns;

	/* Item without upload date should be rejected. */
	VERIFY(!db.item().insert(i));

	/* FIXME : in multi user database this might cause conflict */
	i.uploaded = QDateTime::currentDateTime();

	/* Item with negative quantity should be rejected. */
	i.quantity = -1;
	VERIFY(!db.item().insert(i));
	i.quantity = 0;

	/* Item not yet purchased, should not be in Bought table, nor on stock. */
	i.onStock = true;
	VERIFY(db.item().insert(i));
	/* not in bought table */
	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.size() == 0);
	/* in items table*/
	VERIFY(db.item().query(qtns, is));
	VERIFY(is.size() == 1);
	/* not on stock */
	VERIFY(is.queryAt(0).onStock == false);
	i.onStock = false;
	VERIFY(is.queryAt(0) == i);

	/* Lets clean up */
	VERIFY(db.item().del(i));
	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.size() == 0);

	i.bought = true;

	/* Item having negative price should be rejected. */
	i.shop = "myshop";
	i.price = -1;
	VERIFY(!db.item().insert(i));
	
	/* Item having 0 price should be rejected. */
	i.price = 0;
	VERIFY(!db.item().insert(i));

	/* Item with unregistered shop should be rejected. */
	i.shop = "nonmyshop";
	VERIFY(!db.item().insert(i));
	i.shop = "myshop";

	/* Bought item should be accepted on stock. */
	i.purchased = QDateTime::currentDateTime();
	i.onStock = true;
	VERIFY(db.item().insert(i));
	VERIFY(db.item().query(q, stat, is));

	VERIFY(is.size() == 1);
	VERIFY(i == is.queryAt(0));

	/* Lets clean up */
	VERIFY(db.item().del(i));
	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.size() == 0);

	VERIFY(db.shop().del(s));
	VERIFY(db.shop().query(ss));
	VERIFY(ss.size() == 0);

	VERIFY(db.close());
}

void TestItem::update()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;
	QueryStat stat;

	VERIFY(db.connect());
	VERIFY(db.open());

	Query q("testing query");
	q.startDate = QDateTime(QDate(2000, 1, 1));
	q.endDate = QDateTime(QDate::currentDate().addDays(1));

	/* we will need shop for inserting bought item */
	ShopSet ss;
	Shop s("myshop");
	VERIFY(db.shop().insert(s));

	/* Lets update an inserted item. */
	Item i("orig name");
	Item iu("new name");
	ItemSet is;
	
	i.uploaded = QDateTime::currentDateTime().addDays(-1);
	VERIFY(db.item().insert(i));

	/* Update uploaded field should be rejected. */
	iu.uploaded = QDateTime::currentDateTime();
	VERIFY(!db.item().update(i, iu));
	iu = i;

	/* These fields must be updated */
	iu.name = "updated name";
	iu.category = "updated category";
	iu.bought = true;
	iu.purchased = QDateTime::currentDateTime();
	iu.price = 1;
	iu.shop = "myshop";
	iu.quantity = 2;
	iu.onStock = true;
	iu.comment = "comment";
	VERIFY(db.item().update(i, iu));

	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.size() == 1);
	VERIFY(iu == is.queryAt(0));

	/* Lets clean up */
	VERIFY(db.item().del(iu));
	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.size() == 0);

	VERIFY(db.shop().del(s));
	VERIFY(db.shop().query(ss));
	VERIFY(ss.size() == 0);

	VERIFY(db.close());
}

void TestItem::tagqueries()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;
	QueryStat stat;

	VERIFY(db.connect());
	VERIFY(db.open());

	ItemSet is;

	Query q("testing query");
	q.startDate = QDateTime(QDate(2000, 1, 1));
	q.endDate = QDateTime(QDate::currentDate().addDays(1));

	TagNameSet qtns;

	Tag ta("tag a");
	VERIFY(db.tag().insert(ta));
	
	Tag tb("tag b");
	VERIFY(db.tag().insert(tb));
	
	Ware w("orig name");
	w.tags.add(new QString(tb.name));

	WareSet ws;
	
	VERIFY(db.ware().insert(w));

	Item i("orig name");
	i.uploaded = QDateTime::currentDateTime().addDays(-1);
	i.purchased = QDateTime::currentDateTime();

	VERIFY(db.item().insert(i));

	/* Query item with no tag should has no result set. */
	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.empty());

	/* Query item only with 'tag a' should has no result set. */
	q.withTags.add(new QString(ta.name));
	VERIFY(db.item().query(q, stat, is));
	VERIFY(is.empty());

	/* Query item with 'tag a' and 'tag b' should has result set. */
	q.withTags.add(new QString(tb.name));
	VERIFY(db.item().query(qtns, is));
	VERIFY(is.size() == 1);

	/* Query item with 'tag b' should has result set. */
	q.withTags.remove(ta.name);
	VERIFY(db.item().query(qtns, is));
	VERIFY(is.size() == 1);

	/* FIXME : Not implemented yet. */
	/* Query item with and without 'tag b' should has no result set. */
/*	q.withoutTags.add(new QString(tb.name));
	VERIFY(db.item().query(qtns, is));
	VERIFY(is.size() == 0);
*/
	/* Lets clean up */
	VERIFY(db.item().del(i));
	VERIFY(db.item().query(qtns, is));
	VERIFY(is.size() == 0);

	VERIFY(db.ware().del(w));
	VERIFY(db.ware().query(ws));
	VERIFY(ws.size() == 0);

	VERIFY(db.tag().del(ta));
	VERIFY(db.tag().del(tb));
	TagSet ts;
	VERIFY(db.tag().query(ts));
	VERIFY(is.size() == 0);

	VERIFY(db.close());
}

void TestItem::itemNames()
{
	NOEXC_VERIFY(initTestCase());

	SqliteDb sqliteDb(DB_FILE);
	Db& db = sqliteDb;

	VERIFY(db.connect());
	VERIFY(db.open());

	Item i;

	i.uploaded = QDateTime::currentDateTime().addDays(-1);
	i.name = "i1";
	i.category = "i1c1";
	VERIFY(db.item().insert(i));

	i.uploaded = QDateTime::currentDateTime().addDays(-2);
	i.name = "i1";
	i.category = "i1c2";
	VERIFY(db.item().insert(i));

	i.uploaded = QDateTime::currentDateTime().addDays(-3);
	i.name = "i2";
	i.category = "i1c1";
	VERIFY(db.item().insert(i));

	i.uploaded = QDateTime::currentDateTime().addDays(-4);
	i.name = "i2";
	i.category = "i2c1";
	VERIFY(db.item().insert(i));

	i.uploaded = QDateTime::currentDateTime().addDays(-5);
	i.name = "i2";
	i.category = "i2c2";
	VERIFY(db.item().insert(i));

	i.uploaded = QDateTime::currentDateTime().addDays(-6);
	i.name = "i2";
	i.category = "i2c2";
	VERIFY(db.item().insert(i));

	/* FIXME What on earth I wanted to test with this? */

	/* Delete all the inserted items. */
	ItemSet is;
	TagNameSet tns;
	VERIFY(db.item().query(tns, is));
	VERIFY(is.size() == 6);
	unsigned s = is.size();
	unsigned j;
	for(j=0; j<s; j++)
		VERIFY(db.item().del(is.queryAt(j)));
	
	VERIFY(db.item().query(tns, is));
	VERIFY(is.size() == 0);

	VERIFY(db.close());
}

TEST_INIT(Item)

	TEST_RUN(insert);
	TEST_RUN(update);
	TEST_RUN(tagqueries);
	TEST_RUN(itemNames);

TEST_FINISH(Item)

