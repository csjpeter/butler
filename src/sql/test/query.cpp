/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>

#include <csjp_test.h>

#include <butler_db.h>

#define DB_FILE TESTDIR "/TestSqlite.db"

class TestQuery : public QObject
{
public:
	void initTestCase();
	
	void insert();
	void update();
	void tags();
};


void TestQuery::initTestCase()
{
	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	SqlConnection conn(DB_FILE);
	Db db(conn);

	VERIFY(db.connect());
	VERIFY(db.create());
}

void TestQuery::insert()
{
	NOEXC_VERIFY(initTestCase());

	SqlConnection conn(DB_FILE);
	Db db(conn);

	VERIFY(db.connect());
	VERIFY(db.open());

	Query q("query name");
	QuerySet qs;

	/* Query without dates should be rejected. */
	VERIFY(!db.query().insert(q));

	q.startDate = QDateTime(QDate(2000, 1, 1));
	q.endDate = QDateTime(QDate::currentDate());

	/* Query with dates should be accepted. */
	VERIFY(db.query().insert(q));
	VERIFY(db.query().query(qs));

	VERIFY(qs.size() == 1);
	LOG("Start date: %s", qPrintable(qs.queryAt(0).startDate.toString(Qt::ISODate)));
	LOG("End date: %s", qPrintable(qs.queryAt(0).endDate.toString(Qt::ISODate)));
	VERIFY(q == qs.queryAt(0));

	/* Lets clean up */
	VERIFY(db.query().del(q));
	VERIFY(db.query().query(qs));
	VERIFY(qs.size() == 0);

	VERIFY(db.close());
}

void TestQuery::update()
{
	NOEXC_VERIFY(initTestCase());

	SqlConnection conn(DB_FILE);
	Db db(conn);

	VERIFY(db.connect());
	VERIFY(db.open());

	/* Lets update an inserted query. */
	Query q("orig name");
	Query qu("updated name");
	QuerySet qs;

	q.startDate = QDateTime(QDate(2000, 1, 1));
	q.endDate = QDateTime(QDate::currentDate());

	/* Update to query without dates hould be rejected. */
	VERIFY(db.query().insert(q));
	VERIFY(!db.query().update(q, qu));

	qu.startDate = QDateTime(QDate(2000, 1, 1));
	qu.endDate = QDateTime(QDate::currentDate());

	/* Update to query with dates hould be accepted. */
	VERIFY(db.query().update(q, qu));

	VERIFY(db.query().query(qs));
	VERIFY(qs.size() == 1);
	VERIFY(qu == qs.queryAt(0));

	/* Lets clean up */
	VERIFY(db.query().del(qu));
	VERIFY(db.query().query(qs));
	VERIFY(qs.size() == 0);

	VERIFY(db.close());
}

void TestQuery::tags()
{
	NOEXC_VERIFY(initTestCase());

	SqlConnection conn(DB_FILE);
	Db db(conn);

	VERIFY(db.connect());
	VERIFY(db.open());

	/* Insert query with unregistered tag should fail. */
	Query q("orig name");

	q.startDate = QDateTime(QDate(2000, 1, 1));
	q.endDate = QDateTime(QDate::currentDate());
	q.withTags.add(new QString("unregistered tag"));
	
	VERIFY(!db.query().insert(q));

	q.withTags.removeAt(0);

	/* Insert query with registered tag. */

	Tag registeredTag1("registered tag 1");
	VERIFY(db.tag().insert(registeredTag1));
	Tag registeredTag2("registered tag 2");
	VERIFY(db.tag().insert(registeredTag2));

	q.withTags.add(new QString(registeredTag1.name));

	VERIFY(db.query().insert(q));

	/* Queried Query should have tagset with
	 * 1 tag (registeredTag). */

	QuerySet qs;
	VERIFY(db.query().query(qs));
	VERIFY(qs.size() == 1);
	VERIFY(qs.queryAt(0).withTags.size() == 1);
	VERIFY(qs.queryAt(0).withTags.has("registered tag 1"));

	/* Update query after removing old tag, and adding new one. */

	Query swapped(qs.queryAt(0));
	swapped.withTags.remove("registered tag 1");
	swapped.withTags.add(new QString("registered tag 2"));
	db.query().update(qs.queryAt(0), swapped);

	VERIFY(db.query().query(qs));
	VERIFY(qs.queryAt(0).withTags.size() == 1);
	VERIFY(qs.queryAt(0).withTags.has("registered tag 2"));
	
	VERIFY(db.query().query(qs));
	VERIFY(qs.size() == 1);
	VERIFY(qs.queryAt(0).withTags.size() == 1);
	VERIFY(qs.queryAt(0).withTags.has("registered tag 2"));

	/* Lets clean up */
	VERIFY(db.query().del(q));
	VERIFY(db.query().query(qs));
	VERIFY(qs.size() == 0);

	VERIFY(db.close());
}

TEST_INIT(Query)

	TEST_RUN(insert);
	TEST_RUN(update);
	TEST_RUN(tags);

TEST_FINISH(Query)

