/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

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
