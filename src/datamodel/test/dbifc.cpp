/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_db.h>
#include <butler_item_set.h>
#include <butler_tag_set.h>
#include <butler_query.h>
#include <butler_ware_set.h>

class TagDbImp : public TagDb
{
public:
	TagDbImp(){}
	~TagDbImp(){}

	bool insert(const Tag &t)
	{
		Q_UNUSED(t);
		return true;
	}

	bool update(
		const Tag &orig,
		const Tag &modified)
	{
		Q_UNUSED(orig);
		Q_UNUSED(modified);
		return true;
	}

	bool del(const Tag &t)
	{
		Q_UNUSED(t);
		return true;
	}

	bool query(TagSet &ts)
	{
		Q_UNUSED(ts);

		return true;
	}
};

class QueryDbImp : public QueryDb
{
public:
	QueryDbImp(){}
	~QueryDbImp(){}

	bool insert(const Query &qo)
	{
		Q_UNUSED(qo);
		return true;
	}

	bool update(
		const Query &orig,
		const Query &modified)
	{
		Q_UNUSED(orig);
		Q_UNUSED(modified);
		return true;
	}

	bool del(const Query &qo)
	{
		Q_UNUSED(qo);
		return true;
	}

	bool query(const QString &name, Query &q)
	{
		q.name = name;

		return true;
	}

	bool query(QuerySet &qs)
	{
		Q_UNUSED(qs);

		return true;
	}
};

class ItemDbImp : public ItemDb
{
public:
	ItemDbImp(){}
	~ItemDbImp(){}
	bool insert(const Item &i)
	{
		Q_UNUSED(i);
		return true;
	}

	bool update(
		const Item &orig,
		const Item &modified)
	{
		Q_UNUSED(orig);
		Q_UNUSED(modified);
		return true;
	}

	bool del(const Item &i)
	{
		Q_UNUSED(i);
		return true;
	}

	bool query(const TagNameSet &tns, ItemSet &is)
	{
		Q_UNUSED(tns);
		Q_UNUSED(is);

		return true;
	}

	bool query(const Query &qo, QueryStat &stat, ItemSet &is)
	{
		Q_UNUSED(qo);
		Q_UNUSED(stat);
		Q_UNUSED(is);

		return true;
	}
};

class WareDbImp : public WareDb
{
public:
	WareDbImp(){}
	~WareDbImp(){}

	bool insert(const Ware &w)
	{
		Q_UNUSED(w);
		return true;
	}

	bool update(
		const Ware &orig,
		const Ware &modified)
	{
		Q_UNUSED(orig);
		Q_UNUSED(modified);
		return true;
	}

	bool del(const Ware &w)
	{
		Q_UNUSED(w);
		return true;
	}

	bool query(WareSet &ws)
	{
		Q_UNUSED(ws);

		return true;
	}
};

class ShopDbImp : public ShopDb
{
public:
	ShopDbImp(){}
	~ShopDbImp(){}
	bool insert(const Shop &s)
	{
		Q_UNUSED(s);
		return true;
	}

	bool update(
		const Shop &orig,
		const Shop &modified)
	{
		Q_UNUSED(orig);
		Q_UNUSED(modified);
		return true;
	}

	bool del(const Shop &s)
	{
		Q_UNUSED(s);
		return true;
	}

	bool query(ShopSet &ss)
	{
		Q_UNUSED(ss);

		return true;
	}
};

class DbImp : public Db
{
public:
	DbImp(){}
	~DbImp(){}

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

	bool create()
	{
		return true;
	}

	bool check()
	{
		return true;
	}

	bool update()
	{
		return true;
	}

	TagDb &tag()
	{
		static TagDbImp tag;
		return tag;
	}

	QueryDb &query()
	{
		static QueryDbImp query;
		return query;
	}

	ItemDb &item()
	{
		static ItemDbImp item;
		return item;
	}

	WareDb &ware()
	{
		static WareDbImp ware;
		return ware;
	}

	ShopDb &shop()
	{
		static ShopDbImp shop;
		return shop;
	}
};

/*
 *	Test class
 */

class TestDb
{
	friend class Db;
	friend class TagDb;
	friend class QueryDb;
	friend class ItemDb;
	friend class WareDb;
	friend class ShopDb;
public:	
	void db();
	void tags();
	void query();
	void items();
	void wares();
	void shop();
};

void TestDb::db()
{
	DbImp db;

	db.connect();
	db.open();
	db.close();
	db.create();
	db.check();
	db.update();
}

void TestDb::tags()
{
	DbImp db;

	Tag a, b;
	db.tag().insert(a);
	db.tag().update(a, b);
	db.tag().del(a);

	TagSet ts;
	db.tag().query(ts);
}

void TestDb::query()
{
	DbImp db;

	Query a, b;
	db.query().insert(a);
	db.query().update(a, b);
	db.query().del(a);

	Query qo("default");
	TagSet ts;
	/*
	db.query().query("test", qo);
	VERIFY(qo.name == "test");
	*/
	QuerySet qs;
	db.query().query(qs);
}

void TestDb::items()
{
	DbImp db;
	QueryStat stat;

	Item a, b;
	db.item().insert(a);
	db.item().update(a, b);
	db.item().del(a);

	ItemSet is;
	TagNameSet tns;
	Query qo("default");

	db.item().query(tns, is);

	db.item().query(qo, stat, is);
/*
	WareSet ins;
	db.item().query(ins);
*/
}

void TestDb::wares()
{
	DbImp db;

	Ware a, b;
	db.ware().insert(a);
	db.ware().update(a, b);
	db.ware().del(a);

	WareSet ws;
	db.ware().query(ws);
}

void TestDb::shop()
{
	DbImp db;

	Shop a, b;
	db.shop().insert(a);
	db.shop().update(a, b);
	db.shop().del(a);

	ShopSet ss;
	db.shop().query(ss);
}

TEST_INIT(Db)

	TEST_RUN(db)
	TEST_RUN(tags)
	TEST_RUN(query)
	TEST_RUN(items)
	TEST_RUN(wares)
	TEST_RUN(shop)

TEST_FINISH(Db)

