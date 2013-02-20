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

#include <butler_query.h>

class TestQuery
{
public:
	void construction();
	void equality();
	void comparison();
};

void TestQuery::construction()
{
	/* default constructor*/
	{
		Query qo;

		VERIFY(qo.name == QString(""));
		VERIFY(qo.withTags.size() == 0);
		VERIFY(qo.withoutTags.size() == 0);
	}

	/* parameterized constructor*/
	{
		Query qo("default");

		VERIFY(qo.name == QString("default"));
		VERIFY(qo.withTags.size() == 0);
		VERIFY(qo.withoutTags.size() == 0);
	}

	/* copy constructor*/
	{
		Query qo("default");
		qo.withTags.add(new QString("tag-name"));
		qo.withoutTags.add(new QString("tag-name"));
		qo.startDate = QDateTime(QDate(2000, 1, 1));
		qo.endDate = QDateTime(QDate(2000, 11, 11));

		Query qo_copy(qo);

		VERIFY(qo_copy.name == "default");
		VERIFY(qo_copy.withTags.size() == 1);
		VERIFY(qo_copy.withoutTags.size() == 1);
		VERIFY(qo_copy.startDate == QDateTime(QDate(2000, 1, 1)));
		VERIFY(qo_copy.endDate == QDateTime(QDate(2000, 11, 11)));
	}
}

void TestQuery::equality()
{
	Query a, b;
	
	b.name = "b";
	b.withTags.add(new QString("tag-name"));
	b.withoutTags.add(new QString("tag-name"));
	b.startDate = QDateTime(QDate(2000, 1, 1));
	b.endDate = QDateTime(QDate(2000, 11, 11));
	a = b;
	
	VERIFY(a.name == b.name);
	VERIFY(a.withTags.size() == b.withTags.size());
	VERIFY(a.withoutTags.size() == b.withoutTags.size());
	VERIFY(a.startDate == b.startDate);
	VERIFY(a.endDate == b.endDate);
	VERIFY(a == b);
	VERIFY(!(a != b));

	b.name = "B";

	VERIFY(!(a == b));
	VERIFY(a != b);

	b = a;
	b.withTags.add(new QString("tag-name2"));
	b.withoutTags.add(new QString("tag-name2"));

	VERIFY(!(a == b));
	VERIFY(a != b);

	b = a;
	b.startDate = QDateTime(QDate(2000, 1, 2));

	VERIFY(!(a == b));
	VERIFY(a != b);
	
	b = a;
	b.endDate = QDateTime(QDate(2000, 1, 2));

	VERIFY(!(a == b));
	VERIFY(a != b);
}

void TestQuery::comparison()
{
	/* dont test comparison itself, just consistency */
	Query a("a"), b("B");

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

TEST_INIT(Query)

	TEST_RUN(construction);
	TEST_RUN(equality);
	TEST_RUN(comparison);

TEST_FINISH(Query)
