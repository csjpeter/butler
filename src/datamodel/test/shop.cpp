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

#include <butler_shop.h>

class TestShop
{
public:
	void construction();
	void equality();
	void comparison();
};

void TestShop::construction()
{
	/* default constructor*/
	{
		Shop s;

		VERIFY(s.name == QString(""));
	}

	/* parameterized constructor*/
	{
		Shop s("default");

		VERIFY(s.name == QString("default"));
	}

	/* copy constructor*/
	{
		Shop s;
		s.name = "default";
		s.storeName = "western tesco";
		s.city = "Pecs";
		s.address = "far away";
		s.company = "tesco";

		Shop s_copy(s);

		VERIFY(s_copy.name == "default");
		VERIFY(s_copy.storeName == "western tesco");
		VERIFY(s_copy.city == "Pecs");
		VERIFY(s_copy.address == "far away");
		VERIFY(s_copy.company == "tesco");
	}
}

void TestShop::equality()
{
	Shop a, b;
	a.name = "a";
	b.name = "b";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a.name = b.name;
	a.storeName = "western tesco";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a.storeName = "";
	a.city = "Pecs";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a.city = "";
	a.address = "far away";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a.address = "";
	a.company = "tesco";

	VERIFY(!(a == b));
	VERIFY(a != b);

	a.company = "";

	VERIFY(a == b);
	VERIFY(!(a != b));

	/* operator= */
	{
		Shop s;
		s.name = "default";
		s.storeName = "western tesco";
		s.city = "Pecs";
		s.address = "far away";
		s.company = "tesco";

		Shop s_copy;
		s_copy = s;

		VERIFY(s_copy.name == "default");
		VERIFY(s_copy.storeName == "western tesco");
		VERIFY(s_copy.city == "Pecs");
		VERIFY(s_copy.address == "far away");
		VERIFY(s_copy.company == "tesco");
	}
}

void TestShop::comparison()
{
	/* dont test comparison itself, just consistency */
	Shop a, b;

	a.name = "a";
	b.name = "b";

	/* one of < and > must be true */
	VERIFY(a < b || b < a);
	/* result of < and > must be different */
	VERIFY(!(a < b && b < a));

	b.name = "a";

	VERIFY(!(a < b || b < a));
}

TEST_INIT(Shop)

	TEST_RUN(construction);
	TEST_RUN(equality);
	TEST_RUN(comparison);

TEST_FINISH(Shop)
