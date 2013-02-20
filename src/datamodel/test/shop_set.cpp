/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,ss,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>
#include <csjp_debug.h>

#include <butler_shop_set.h>

class TestShopSet
{
public:
	void construction();
	void insertion();
	void removation();
	void copy();
	void query();
	void equality();
};

void TestShopSet::construction()
{
	ShopSet ss;

	VERIFY(ss.size() == 0);
	VERIFY(ss.empty() == true);
}

void TestShopSet::insertion()
{
	Shop *s = new Shop("first");
	ShopSet ss;
	ss.add(s);

	VERIFY(ss.size() == 1);

	const Shop &q = ss.queryAt(0);

	VERIFY(q.name == "first");
}

void TestShopSet::removation()
{
	ShopSet ss;
	Shop *item;
	
	item = new Shop("first");
	ss.add(item);

	item = new Shop("second");
	ss.add(item);

	item = new Shop("third");
	ss.add(item);

	ss.removeAt(1);

	VERIFY(ss.size() == 2);
	VERIFY(ss.queryAt(0).name == "first");
	VERIFY(ss.queryAt(1).name == "third");
}

void TestShopSet::copy()
{
	Shop *s = new Shop("first");
	ShopSet ss;
	ss.add(s);

	{
		ShopSet ss_copy(ss);

		VERIFY(ss_copy.size() == 1);
		VERIFY(ss_copy.empty() == false);
		VERIFY(ss.queryAt(0) == ss_copy.queryAt(0));
	}

	{
		ShopSet ss_copy;
		ss_copy.add(new Shop("old"));
		ss_copy.copy(ss);

		VERIFY(ss_copy.size() == 1);
		VERIFY(ss_copy.empty() == false);
		VERIFY(ss.queryAt(0) == ss_copy.queryAt(0));
	}
}

void TestShopSet::query()
{
	ShopSet ss;
	Shop *item;

	item = new Shop("first");
	ss.add(item);

	item = new Shop("second");
	ss.add(item);

	item = new Shop("third");
	ss.add(item);

	{
		Shop &s = ss.query("first");
		VERIFY(s.name == "first");
	}

	{
		int i = ss.index("second");
		VERIFY(i == 1);

		Shop &a = ss.queryAt(i);
		Shop &b = ss.query("second");
		VERIFY(a.name == b.name);
	}

	{
		VERIFY(ss.has("third"));
	}
}

void TestShopSet::equality()
{
	ShopSet a, b;
	
	a.add(new Shop("xyz"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.add(new Shop("xy"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.clear();
	b.add(new Shop("xyz"));

	VERIFY(a == b);
	VERIFY(!(a != b));

	VERIFY(a.queryAt(0) == b.queryAt(0));
	VERIFY(a.size() == b.size());
}

TEST_INIT(ShopSet)

	TEST_RUN(construction);
	TEST_RUN(insertion);
	TEST_RUN(removation);
	TEST_RUN(copy);
	TEST_RUN(query);
	TEST_RUN(equality);

TEST_FINISH(ShopSet)

