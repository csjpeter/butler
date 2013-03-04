/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_item.h>

class TestItem
{
public:
	void construction();
	void equality();
	void comparison();
};

void TestItem::construction()
{
	/* default constructor*/
	{
		Item i;

		VERIFY(i.name == QString(""));
	}

	/* parameterized constructor*/
	{
		Item i("default");

		VERIFY(i.name == QString("default"));
	}

	/* copy constructor*/
	{
		Item i;
		i.name = "default";

		Item i_copy(i);

		VERIFY(i_copy.name == "default");
	}
}

void TestItem::equality()
{
	Item a, b;
	a.uploaded.setDate(QDate(2009, 12, 10));
	a.uploaded.setTime(QTime(0, 0, 0));
	b.uploaded.setDate(QDate(2010, 12, 10));
	b.uploaded.setTime(QTime(0, 0, 0));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.uploaded.setDate(QDate(2008, 12, 10));
	a.uploaded.setDate(QDate(2008, 10, 1));
	a.category = "delete this";
	a.quantity = 100;
	a.comment = QString("delete this");
	a.bought = true;
	a.purchased.setDate(QDate(2008, 12, 1));
	a.shop = "shop";
	a.price = -1;
	a.onStock = true;
	a = b;

	VERIFY(a.name == b.name);
	VERIFY(a.category == b.category);
	VERIFY(a.quantity == b.quantity);
	VERIFY(a.comment == b.comment);

	VERIFY(a.bought == b.bought);
	VERIFY(a.purchased == b.purchased);
	VERIFY(a.shop == b.shop);
	VERIFY(a.price == b.price);
	VERIFY(a.onStock == b.onStock);
}

void TestItem::comparison()
{
	/* dont test comparison itself, just consistency */
	Item a, b;

	a.uploaded.setDate(QDate(2009, 12, 10));
	a.uploaded.setTime(QTime(0, 0, 0));
	b.uploaded.setDate(QDate(2009, 12, 11));
	b.uploaded.setTime(QTime(0, 0, 0));

	/* one of < and > must be true */
	VERIFY(a < b || b < a);
	/* result of < and > must be different */
	VERIFY(!(a < b && b < a));

	a.uploaded.setDate(QDate(2000, 1, 1));
	b.uploaded.setDate(QDate(2000, 1, 1));

	VERIFY(!(a < b || b < a));

	a.uploaded.setDate(QDate(2008, 10, 10));
	b.uploaded.setDate(QDate(2008, 10, 11));

	VERIFY(a < b || b < a);
	VERIFY(!(a < b && b < a));
}

TEST_INIT(Item)

	TEST_RUN(construction);
	TEST_RUN(equality);
	TEST_RUN(comparison);

TEST_FINISH(Item)
