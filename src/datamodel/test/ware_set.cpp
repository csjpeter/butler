/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_ware_set.h>

class TestWareSet
{
public:
	void construction();
	void insertion();
	void removation();
	void copy();
	void query();
	void equality();
};

void TestWareSet::construction()
{
	WareSet s;

	VERIFY(s.size() == 0);
	VERIFY(s.empty() == true);
}

void TestWareSet::insertion()
{
	Ware *in = new Ware("first");
	WareSet s;
	s.add(in);

	VERIFY(s.size() == 1);
	VERIFY(s.has("first"));

	const Ware &q = s.queryAt(0);

	VERIFY(q.name == "first");
}

void TestWareSet::removation()
{
	WareSet s;
	s.add(new Ware("first"));
	s.add(new Ware("second"));
	s.add(new Ware("third"));

	s.removeAt(1);

	VERIFY(s.size() == 2);
	VERIFY(s.queryAt(0).name == "first");
	VERIFY(s.queryAt(1).name == "third");
}

void TestWareSet::copy()
{
	Ware *in = new Ware("first");
	WareSet s;
	s.add(in);

	{
		WareSet s_copy(s);

		VERIFY(s_copy.size() == 1);
		VERIFY(s_copy.empty() == false);
		VERIFY(s.queryAt(0) == s_copy.queryAt(0));
	}

	{
		WareSet s_copy;
		s_copy.add(new Ware("old"));
		s_copy.copy(s);

		VERIFY(s_copy.size() == 1);
		VERIFY(s_copy.empty() == false);
		VERIFY(s.queryAt(0) == s_copy.queryAt(0));
	}
}

void TestWareSet::query()
{
	WareSet s;
	s.add(new Ware("0zeroth"));
	s.add(new Ware("1first"));
	s.add(new Ware("2second"));
	s.add(new Ware("3third"));
	s.add(new Ware("4forth"));
	s.add(new Ware("5fifth"));
	s.add(new Ware("6sixth"));

	{
		Ware &in = s.query("3third");
		VERIFY(in.name == "3third");
	}

	{
		int i = s.index("4forth");
		VERIFY(i == 4);

		Ware &a = s.queryAt(i);
		Ware &b = s.query("4forth");
		VERIFY(a.name == b.name);
	}

	{
		VERIFY(!s.has("7seventh"));
	}
}

void TestWareSet::equality()
{
	WareSet a, b;
	
	a.add(new Ware("xyz"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.add(new Ware("xy"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.clear();
	b.add(new Ware("xyz"));

	VERIFY(a == b);
	VERIFY(!(a != b));

	VERIFY(a.queryAt(0) == b.queryAt(0));
	VERIFY(a.size() == b.size());
}

TEST_INIT(WareSet)

	TEST_RUN(construction);
	TEST_RUN(insertion);
	TEST_RUN(removation);
	TEST_RUN(copy);
	TEST_RUN(query);
	TEST_RUN(equality);

TEST_FINISH(WareSet)
