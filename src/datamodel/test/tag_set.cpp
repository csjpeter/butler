/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>
#include <csjp_debug.h>

#include <butler_tag_set.h>

class TestTagSet
{
public:
	void construction();
	void insertion();
	void removation();
	void copy();
	void query();
	void equality();
};

void TestTagSet::construction()
{
	TagSet ts;

	VERIFY(ts.size() == 0);
	VERIFY(ts.empty() == true);
}

void TestTagSet::insertion()
{
	Tag *t = new Tag("first");
	TagSet ts;
	ts.add(t);

	VERIFY(ts.size() == 1);
	VERIFY(ts.has("first"));

	const Tag &q = ts.queryAt(0);

	VERIFY(q.name == "first");
}

void TestTagSet::removation()
{
	TagSet ts;
	ts.add(new Tag("first"));
	ts.add(new Tag("second"));
	ts.add(new Tag("third"));

	ts.removeAt(1);

	VERIFY(ts.size() == 2);
	VERIFY(ts.queryAt(0).name == "first");
	VERIFY(ts.queryAt(1).name == "third");
}

void TestTagSet::copy()
{
	Tag *t = new Tag("first");
	TagSet ts;
	ts.add(t);

	{
		TagSet ts_copy(ts);

		VERIFY(ts_copy.size() == 1);
		VERIFY(ts_copy.empty() == false);
		VERIFY(ts.queryAt(0) == ts_copy.queryAt(0));
	}

	{
		TagSet ts_copy;
		ts_copy.add(new Tag("old"));
		ts_copy.copy(ts);

		VERIFY(ts_copy.size() == 1);
		VERIFY(ts_copy.empty() == false);
		VERIFY(ts.queryAt(0) == ts_copy.queryAt(0));
	}
}

void TestTagSet::query()
{
	TagSet ts;
	ts.add(new Tag("0zeroth"));
	ts.add(new Tag("1first"));
	ts.add(new Tag("2second"));
	ts.add(new Tag("3third"));
	ts.add(new Tag("4forth"));
	ts.add(new Tag("5fifth"));
	ts.add(new Tag("6sixth"));

	{
		Tag &t = ts.query("3third");
		VERIFY(t.name == "3third");
	}

	{
		int i = ts.index("4forth");
		VERIFY(i == 4);

		Tag &a = ts.queryAt(i);
		Tag &b = ts.query("4forth");
		VERIFY(a.name == b.name);
	}

	{
		VERIFY(!ts.has("7seventh"));
	}
}

void TestTagSet::equality()
{
	TagSet a, b;
	
	a.add(new Tag("xyz"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.add(new Tag("xy"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.clear();
	b.add(new Tag("xyz"));

	VERIFY(a == b);
	VERIFY(!(a != b));

	VERIFY(a.queryAt(0) == b.queryAt(0));
	VERIFY(a.size() == b.size());
}

TEST_INIT(TagSet)

	TEST_RUN(construction);
	TEST_RUN(insertion);
	TEST_RUN(removation);
	TEST_RUN(copy);
	TEST_RUN(query);
	TEST_RUN(equality);

TEST_FINISH(TagSet)
