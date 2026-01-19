/*
 * Author: Butler Test Suite
 * Unit tests for butler_conversions.h
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_conversions.h>
#include <QString>
#include <QVariant>
#include <QDateTime>

class TestConversions
{
public:
	void testStringQStringConversions();
	void testQVariantConversions();
	void testNumericConversions();
	void testDateTimeConversions();
};

void TestConversions::testStringQStringConversions()
{
	TESTSTEP("Test csjp::String to QString");
	csjp::String cstr("Hello World");
	QString qstr;
	qstr <<= cstr;
	VERIFY(qstr == "Hello World");
	
	TESTSTEP("Test QString to csjp::String");
	QString qstr2("Test String");
	csjp::String cstr2;
	cstr2 <<= qstr2;
	VERIFY(cstr2 == "Test String");
	
	TESTSTEP("Test QString append to csjp::String");
	csjp::String cstr3("Start ");
	QString qstr3("End");
	cstr3 << qstr3;
	VERIFY(cstr3 == "Start End");
}

void TestConversions::testQVariantConversions()
{
	TESTSTEP("Test bool to QVariant");
	QVariant var;
	var <<= true;
	VERIFY(var.toBool() == true);
	
	TESTSTEP("Test int to QVariant");
	QVariant var2;
	var2 <<= 42;
	VERIFY(var2.toInt() == 42);
	
	TESTSTEP("Test unsigned to QVariant");
	QVariant var3;
	unsigned val = 123u;
	var3 <<= val;
	VERIFY(var3.toUInt() == 123u);
	
	TESTSTEP("Test double to QVariant");
	QVariant var4;
	var4 <<= 3.14;
	VERIFY(var4.toDouble() == 3.14);
	
	TESTSTEP("Test QString to QVariant");
	QVariant var5;
	QString qstr("test");
	var5 <<= qstr;
	VERIFY(var5.toString() == "test");
	
	TESTSTEP("Test csjp::String to QVariant");
	QVariant var6;
	csjp::String cstr("csjp test");
	var6 <<= cstr;
	VERIFY(var6.toString() == "csjp test");
}

void TestConversions::testNumericConversions()
{
	TESTSTEP("Test QVariant to bool");
	QVariant var(true);
	bool b = false;
	b <<= var;
	VERIFY(b == true);
	
	TESTSTEP("Test QVariant to int");
	QVariant var2(100);
	int i = 0;
	i <<= var2;
	VERIFY(i == 100);
	
	TESTSTEP("Test QVariant to unsigned");
	QVariant var3(200u);
	unsigned u = 0;
	u <<= var3;
	VERIFY(u == 200u);
	
	TESTSTEP("Test QVariant to double");
	QVariant var4(2.718);
	double d = 0.0;
	d <<= var4;
	VERIFY(d == 2.718);
	
	TESTSTEP("Test unsigned to csjp::String");
	csjp::String str;
	unsigned val = 456u;
	str <<= val;
	VERIFY(str.length > 0);
}

void TestConversions::testDateTimeConversions()
{
	TESTSTEP("Test QDateTime to QVariant");
	QDateTime qdt = QDateTime::fromString("2023-05-15T10:30:00", Qt::ISODate);
	QVariant var;
	var <<= qdt;
	VERIFY(var.toDateTime() == qdt);
	
	TESTSTEP("Test QVariant to QDateTime");
	QVariant var2(qdt);
	QDateTime qdt2;
	qdt2 <<= var2;
	VERIFY(qdt2 == qdt);
	
	TESTSTEP("Test QVariant to QString");
	QVariant var3("datetime test");
	QString qstr;
	qstr <<= var3;
	VERIFY(qstr == "datetime test");
	
	TESTSTEP("Test QVariant to csjp::String");
	QVariant var4("conversion test");
	csjp::String cstr;
	cstr <<= var4;
	VERIFY(cstr == "conversion test");
}

TEST_INIT(Conversions)
	TEST_RUN(testStringQStringConversions);
	TEST_RUN(testQVariantConversions);
	TEST_RUN(testNumericConversions);
	TEST_RUN(testDateTimeConversions);
TEST_FINISH(Conversions)
