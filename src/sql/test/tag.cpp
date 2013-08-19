/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QFile>

#include <csjp_test.h>

#include <butler_db.h>

#define DB_FILE TESTDIR "/TestSqlite.db"

DatabaseDescriptor sqliteDesc;

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
	sqliteDesc.name = "localdb";
	sqliteDesc.driver = "QSQLITE";
	sqliteDesc.databaseName = DB_FILE;

	QFile f(DB_FILE);
	if(f.exists())
		VERIFY(f.remove());

	Db db(sqliteDesc);

}

void TestTag::database()
{
	NOEXC_VERIFY(initTestCase());

	Db db(sqliteDesc);

	VERIFY(db.open());

	/* Are checks working on a fresh database? */
	VERIFY(db.check());

}

void TestTag::insert()
{
	NOEXC_VERIFY(initTestCase());

	Db db(sqliteDesc);

	VERIFY(db.open());

	/* Lets test insert and query of some tricky name. */
	Tag t("\"name's");
	TagSet ts;

	/* Check if we can insert a new tag. */
	VERIFY(db.tag.insert(t));
	VERIFY(db.tag.query(ts));

	VERIFY(ts.size() == 1);
	VERIFY(t == ts.queryAt(0));

	/* Lets clean up */
	VERIFY(db.tag.del(t));
	VERIFY(db.tag.query(ts));
	VERIFY(ts.size() == 0);

}

void TestTag::update()
{
	NOEXC_VERIFY(initTestCase());

	Db db(sqliteDesc);

	VERIFY(db.open());

	/* Lets update an inserted tag. */
	Tag t("orig name");
	Tag tu("updated name");
	TagSet ts;

	/* Check if we can change the name of an existing tag. */
	VERIFY(db.tag.insert(t));
	VERIFY(db.tag.update(t, tu));
	VERIFY(db.tag.query(ts));

	VERIFY(ts.size() == 1);
	VERIFY(tu == ts.queryAt(0));

	/* Lets clean up */
	VERIFY(db.tag.del(tu));
	VERIFY(db.tag.query(ts));
	VERIFY(ts.size() == 0);

}

TEST_INIT(Tag)

	TEST_RUN(database);
	TEST_RUN(insert);
	TEST_RUN(update);

TEST_FINISH(Tag)

