/*
 * Author: Butler Test Suite
 * Unit tests for butler_dataclasses.cpp
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_dataclasses.h>

class TestDataClasses
{
public:
	void testTagConstruction();
	void testItemQueryStat();
	void testPaymentQueryStat();
};

void TestDataClasses::testTagConstruction()
{
	TESTSTEP("Test Tag default construction");
	Tag tag1;
	VERIFY(tag1.name.length() == 0);

	TESTSTEP("Test Tag construction with name");
	Text tagName("test-tag");
	Tag tag2(tagName);
	VERIFY(tag2.name == tagName);

	TESTSTEP("Test Tag copy construction");
	Tag tag3(tag2);
	VERIFY(tag3.name == tag2.name);
}

void TestDataClasses::testItemQueryStat()
{
	TESTSTEP("Test ItemQueryStat initialization");
	ItemQueryStat stat;
	VERIFY(stat.cheapestUnitPrice == DBL_MAX);
}

void TestDataClasses::testPaymentQueryStat()
{
	TESTSTEP("Test PaymentQueryStat initialization");
	PaymentQueryStat stat;
	VERIFY(stat.cheapestPrice == DBL_MAX);
}

TEST_INIT(DataClasses)
	TEST_RUN(testTagConstruction);
	TEST_RUN(testItemQueryStat);
	TEST_RUN(testPaymentQueryStat);
TEST_FINISH(DataClasses)
