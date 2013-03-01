/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_tag.h>

class TestTag
{
public:
	void construction();
	void equality();
	void comparison();
};

void TestTag::construction()
{
	/* default constructor*/
	{
		Tag tag;

		VERIFY(tag.name == QString(""));
	}

	/* parameterized constructor*/
	{
		Tag tag("tag-name");

		VERIFY(tag.name == QString("tag-name"));
	}

	/* copy constructor*/
	{
		Tag t("tag-name");
		Tag tag(t);

		VERIFY(tag.name == t.name);
		VERIFY(tag.description == t.description);
	}
}

void TestTag::equality()
{
	Tag a("a"), b("A");

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.name = "a";
	b.description = "d";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a.description = "d";

	VERIFY(a == b);
	VERIFY(!(a != b));

	a.description = "";
	a.name = "b";
	a = b;

	VERIFY(a.name == b.name);
	VERIFY(a.description == b.description);
}

void TestTag::comparison()
{
	/* dont test comparison itself, just consistency */
	Tag a("a"), b("B");

	/* one of < and > must be true */
	VERIFY(a < b || b < a);
	/* result of < and > must be different */
	VERIFY(!(a < b && b < a));

	a.name = "az3d";
	b.name = "az3d";

	VERIFY(!(a < b || b < a));

	a.name = "óa";
	b.name = "oB";

	VERIFY(a < b || b < a);
	VERIFY(!(a < b && b < a));
}

TEST_INIT(Tag)

	TEST_RUN(construction);
	TEST_RUN(equality);
	TEST_RUN(comparison);

TEST_FINISH(Tag)

