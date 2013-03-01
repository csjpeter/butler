/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_ware.h>

class TestWare
{
public:
	void construction();
	void equality();
	void comparison();
};

void TestWare::construction()
{
	/* default constructor*/
	{
		Ware name;

		VERIFY(name.name == QString(""));
	}

	/* parameterized constructor*/
	{
		Ware name("name");

		VERIFY(name.name == QString("name"));
	}

	/* copy constructor*/
	{
		Ware n("name");
		Ware name(n);

		VERIFY(name.name == n.name);
	}
}

void TestWare::equality()
{
	Ware a("a"), b("A");

	VERIFY(!(a == b));
	VERIFY(a != b);

	a = b;
	
	VERIFY(a == b);
	VERIFY(!(a != b));
	
	b.categories.add(new QString("category"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	a = b;

	VERIFY(a == b);
	VERIFY(!(a != b));

	b.tags.add(new QString("tag"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	a = b;

	VERIFY(a == b);
	VERIFY(!(a != b));

	b.unit = "unit";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a = b;

	VERIFY(a == b);
	VERIFY(!(a != b));
}

void TestWare::comparison()
{
	/* dont test comparison itself, just consistency */
	Ware a("a"), b("B");

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

TEST_INIT(Ware)

	TEST_RUN(construction);
	TEST_RUN(equality);
	TEST_RUN(comparison);

TEST_FINISH(Ware)
