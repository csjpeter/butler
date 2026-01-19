/*
 * Author: Butler Test Suite
 * Unit tests for butler_datetime.h
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_datetime.h>
#include <QDateTime>
#include <QDate>
#include <QTime>

class TestDateTime
{
public:
	void testDefaultConstruction();
	void testStringConstruction();
	void testCopyConstruction();
	void testAssignment();
	void testComparison();
	void testIsoUtcString();
	void testNow();
};

void TestDateTime::testDefaultConstruction()
{
	TESTSTEP("Test DateTime default construction");
	DateTime dt;
	VERIFY(dt.date() == QDate(2000, 1, 1));
	VERIFY(dt.time() == QTime(0, 0, 0));
}

void TestDateTime::testStringConstruction()
{
	TESTSTEP("Test DateTime construction from ISO string");
	DateTime dt("2023-12-25T10:30:00");
	VERIFY(dt.date().year() == 2023);
	VERIFY(dt.date().month() == 12);
	VERIFY(dt.date().day() == 25);
	VERIFY(dt.time().hour() == 10);
	VERIFY(dt.time().minute() == 30);
	
	TESTSTEP("Test DateTime construction from csjp::String");
	csjp::String str("2024-01-15T14:45:30");
	DateTime dt2(str);
	VERIFY(dt2.date().year() == 2024);
	VERIFY(dt2.date().month() == 1);
	VERIFY(dt2.date().day() == 15);
}

void TestDateTime::testCopyConstruction()
{
	TESTSTEP("Test DateTime copy construction");
	DateTime dt1("2023-06-15T12:00:00");
	DateTime dt2(dt1);
	VERIFY(dt2.date() == dt1.date());
	VERIFY(dt2.time() == dt1.time());
	
	TESTSTEP("Test DateTime construction from QDateTime");
	QDateTime qdt = QDateTime::fromString("2023-07-20T08:30:00", Qt::ISODate);
	DateTime dt3(qdt);
	VERIFY(dt3.date() == qdt.date());
	VERIFY(dt3.time() == qdt.time());
}

void TestDateTime::testAssignment()
{
	TESTSTEP("Test DateTime assignment from string");
	DateTime dt;
	dt = "2023-11-10T16:20:00";
	VERIFY(dt.date().year() == 2023);
	VERIFY(dt.date().month() == 11);
	
	TESTSTEP("Test DateTime assignment from QDateTime");
	QDateTime qdt = QDateTime::fromString("2024-02-28T09:15:00", Qt::ISODate);
	DateTime dt2;
	dt2 = qdt;
	VERIFY(dt2.date() == qdt.date());
	VERIFY(dt2.time() == qdt.time());
	
	TESTSTEP("Test DateTime assignment from DateTime");
	DateTime dt3;
	dt3 = dt;
	VERIFY(dt3.date() == dt.date());
	VERIFY(dt3.time() == dt.time());
}

void TestDateTime::testComparison()
{
	TESTSTEP("Test DateTime equality");
	DateTime dt1("2023-05-10T12:00:00");
	DateTime dt2("2023-05-10T12:00:00");
	VERIFY(dt1.isEqual(dt2));
	VERIFY(dt1 == dt2);
	
	TESTSTEP("Test DateTime inequality");
	DateTime dt3("2023-05-11T12:00:00");
	VERIFY(dt1 != dt3);
	
	TESTSTEP("Test DateTime less than");
	VERIFY(dt1.isLess(dt3));
	VERIFY(dt1 < dt3);
	
	TESTSTEP("Test DateTime greater than");
	VERIFY(dt3.isMore(dt1));
}

void TestDateTime::testIsoUtcString()
{
	TESTSTEP("Test DateTime isoUtcString");
	DateTime dt("2023-08-15T10:30:00");
	QString isoStr = dt.isoUtcString();
	VERIFY(isoStr.contains("2023-08-15"));
	VERIFY(isoStr.endsWith("Z"));
}

void TestDateTime::testNow()
{
	TESTSTEP("Test DateTime::now()");
	DateTime before = DateTime::now();
	// Small delay to ensure time difference
	for(volatile int i = 0; i < 1000000; i++);
	DateTime after = DateTime::now();
	
	// After should be greater than or equal to before
	VERIFY(!(after < before));
}

TEST_INIT(DateTime)
	TEST_RUN(testDefaultConstruction);
	TEST_RUN(testStringConstruction);
	TEST_RUN(testCopyConstruction);
	TEST_RUN(testAssignment);
	TEST_RUN(testComparison);
	TEST_RUN(testIsoUtcString);
	TEST_RUN(testNow);
TEST_FINISH(DateTime)
