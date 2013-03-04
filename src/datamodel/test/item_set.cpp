/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_item_set.h>

class TestItemSet
{
public:
	void construction();
	void insertion();
	void removation();
	void copy();
	void query();
	void equality();
};

void TestItemSet::construction()
{
	ItemSet is;

	VERIFY(is.size() == 0);
	VERIFY(is.empty() == true);
}

void TestItemSet::insertion()
{
	Item *i = new Item("first");
	ItemSet is;
	is.add(i);

	VERIFY(is.size() == 1);

	const Item &q = is.queryAt(0);

	VERIFY(q.name == "first");
}

void TestItemSet::removation()
{
	ItemSet is;
	Item *item;

	is.ordering = Item::Uploaded;

	item = new Item("first");
	item->uploaded = QDateTime::currentDateTime().addDays(-3);
	is.add(item);

	item = new Item("second");
	item->uploaded = QDateTime::currentDateTime().addDays(-2);
	is.add(item);

	item = new Item("third");
	item->uploaded = QDateTime::currentDateTime().addDays(-1);
	is.add(item);

	VERIFY(is.queryAt(0).name == "first");
	VERIFY(is.queryAt(1).name == "second");
	VERIFY(is.queryAt(2).name == "third");

	is.removeAt(1);

	VERIFY(is.size() == 2);
	VERIFY(is.queryAt(0).name == "first");
	VERIFY(is.queryAt(1).name == "third");
}

void TestItemSet::copy()
{
	ItemSet is;
	is.add(new Item("first"));

	{
		ItemSet is_copy(is);

		VERIFY(is_copy.size() == 1);
		VERIFY(is_copy.empty() == false);
		VERIFY(is.queryAt(0) == is_copy.queryAt(0));
	}

	{
		ItemSet is_copy;
		is_copy.add(new Item("old"));
		is_copy.copy(is);

		VERIFY(is_copy.size() == 1);
		VERIFY(is_copy.empty() == false);
		VERIFY(is.queryAt(0) == is_copy.queryAt(0));
	}
}

void TestItemSet::query()
{
	ItemSet is;
	csjp::Object<Item> item;
	QDateTime dt;

	item.ptr = new Item("first");
	item->uploaded = QDateTime::currentDateTime().addDays(-1);
	is.add(item);

	item.ptr = new Item("second");
	item->uploaded = QDateTime::currentDateTime().addDays(-2);
	dt = item->uploaded;
	is.add(item);

	item.ptr = new Item("third");
	item->uploaded = QDateTime::currentDateTime().addDays(-3);
	is.add(item);

	{
		Item &i = is.query(dt);
		VERIFY(i.name == "second");
	}

	{
		int i = is.index(dt);
		VERIFY(i == 1);

		Item &a = is.queryAt(i);
		Item &b = is.query(dt);
		VERIFY(a.name == b.name);
	}

	{
		VERIFY(!is.has(QDateTime::currentDateTime()));
	}
}

void TestItemSet::equality()
{
	ItemSet a, b;

	a.add(new Item("xyz"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.add(new Item("xy"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.clear();
	b.add(new Item("xyz"));

	VERIFY(a == b);
	VERIFY(!(a != b));

	VERIFY(a.queryAt(0) == b.queryAt(0));
	VERIFY(a.size() == b.size());
}

TEST_INIT(ItemSet)

	TEST_RUN(construction);
	TEST_RUN(insertion);
	TEST_RUN(removation);
	TEST_RUN(copy);
	TEST_RUN(query);
	TEST_RUN(equality);

TEST_FINISH(ItemSet)
