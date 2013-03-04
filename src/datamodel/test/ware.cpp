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
