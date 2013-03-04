/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <stdio.h>

#include <csjp_test.h>

#include <butler_query_set.h>

class TestQuerySet
{
public:
	void construction();
	void insertion();
	void removation();
	void copy();
	void equality();
};

void TestQuerySet::construction()
{
	QuerySet qos;

	VERIFY(qos.size() == 0);
	VERIFY(qos.empty() == true);
}

void TestQuerySet::insertion()
{
	Query *qo = new Query("first");
	QuerySet qos;
	qos.add(qo);

	VERIFY(qos.size() == 1);

	const Query &q = qos.queryAt(0);

	VERIFY(q.name == "first");
}

void TestQuerySet::removation()
{
	QuerySet qos;
	qos.add(new Query("first"));
	qos.add(new Query("second"));
	qos.add(new Query("third"));

	qos.removeAt(1);

	VERIFY(qos.size() == 2);
	VERIFY(qos.queryAt(0).name == "first");
	VERIFY(qos.queryAt(1).name == "third");
}

void TestQuerySet::copy()
{
	Query *qo = new Query("first");
	QuerySet qos;
	qos.add(qo);

	{
		QuerySet qos_copy(qos);

		VERIFY(qos_copy.size() == 1);
		VERIFY(qos_copy.empty() == false);
		VERIFY(qos.queryAt(0) == qos_copy.queryAt(0));
	}

	{
		QuerySet qos_copy;
		qos_copy.add(new Query("old"));
		qos_copy.copy(qos);

		VERIFY(qos_copy.size() == 1);
		VERIFY(qos_copy.empty() == false);
		VERIFY(qos.queryAt(0) == qos_copy.queryAt(0));
	}
}

void TestQuerySet::equality()
{
	QuerySet a, b;

	a.add(new Query("xyz"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.add(new Query("xy"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b.clear();
	b.add(new Query("xyz"));

	VERIFY(a == b);
	VERIFY(!(a != b));

	VERIFY(a.queryAt(0) == b.queryAt(0));
	VERIFY(a.size() == b.size());
}

TEST_INIT(QuerySet)

	TEST_RUN(construction);
	TEST_RUN(insertion);
	TEST_RUN(removation);
	TEST_RUN(copy);
	TEST_RUN(equality);

TEST_FINISH(QuerySet)
