/*
 * Author: Butler Test Suite
 * Unit tests for butler_types.h
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_types.h>
#include <QVariant>

class TestTypes
{
public:
	void testSqlDriverEnum();
	void testSqlDriverConversions();
	void testYNBoolConversions();
	void testItemQueryStockOptions();
	void testItemQueryTagOptions();
	void testStringConversions();
};

void TestTypes::testSqlDriverEnum()
{
	TESTSTEP("Test SqlDriver enum creation");
	SqlDriver driver1;
	SqlDriver driver2(SqlDriver::Enum::SQLite);
	VERIFY(driver2 == SqlDriver::Enum::SQLite);
	
	TESTSTEP("Test SqlDriver copy construction");
	SqlDriver driver3(driver2);
	VERIFY(driver3 == SqlDriver::Enum::SQLite);
	
	TESTSTEP("Test SqlDriver assignment");
	SqlDriver driver4;
	driver4 = driver2;
	VERIFY(driver4 == SqlDriver::Enum::SQLite);
}

void TestTypes::testSqlDriverConversions()
{
	TESTSTEP("Test SqlDriver to csjp::String");
	SqlDriver driver(SqlDriver::Enum::SQLite);
	csjp::String str;
	str <<= driver;
	VERIFY(str == "SQLite");
	
	TESTSTEP("Test SqlDriver to QVariant");
	QVariant var;
	var <<= driver;
	VERIFY(var.toString() == "SQLite");
	
	TESTSTEP("Test QVariant to SqlDriver");
	QVariant var2("SQLite");
	SqlDriver driver2;
	driver2 <<= var2;
	VERIFY(driver2 == SqlDriver::Enum::SQLite);
	
	TESTSTEP("Test SqlDriver string comparison");
	VERIFY(driver == "SQLite");
}

void TestTypes::testYNBoolConversions()
{
	TESTSTEP("Test YNBool to QVariant");
	YNBool yn;
	yn.val = true;
	QVariant var;
	var <<= yn;
	VERIFY(var.toString() == "Y");
	
	YNBool yn2;
	yn2.val = false;
	QVariant var2;
	var2 <<= yn2;
	VERIFY(var2.toString() == "N");
	
	TESTSTEP("Test QVariant to YNBool");
	QVariant varTrue(true);
	YNBool ynTrue;
	ynTrue <<= varTrue;
	VERIFY(ynTrue.val == true);
	
	QVariant varFalse(false);
	YNBool ynFalse;
	ynFalse <<= varFalse;
	VERIFY(ynFalse.val == false);
}

void TestTypes::testItemQueryStockOptions()
{
	TESTSTEP("Test ItemQueryStockOptions to csjp::String");
	csjp::String str;
	str <<= ItemQueryStockOptions::AllItemChanges;
	VERIFY(str == "AllItemChanges");
	
	str <<= ItemQueryStockOptions::Gains;
	VERIFY(str == "Gains");
	
	str <<= ItemQueryStockOptions::Looses;
	VERIFY(str == "Looses");
	
	TESTSTEP("Test ItemQueryStockOptions to QString");
	QString qstr;
	qstr <<= ItemQueryStockOptions::Gains;
	VERIFY(qstr == "Gains");
	
	TESTSTEP("Test QVariant to ItemQueryStockOptions");
	QVariant var("Looses");
	ItemQueryStockOptions opt = ItemQueryStockOptions::AllItemChanges;
	opt <<= var;
	VERIFY(opt == ItemQueryStockOptions::Looses);
	
	TESTSTEP("Test csjp::CString to ItemQueryStockOptions");
	ItemQueryStockOptions opt2 = ItemQueryStockOptions::AllItemChanges;
	opt2 <<= csjp::CString("Gains");
	VERIFY(opt2 == ItemQueryStockOptions::Gains);
}

void TestTypes::testItemQueryTagOptions()
{
	TESTSTEP("Test ItemQueryTagOptions to csjp::String");
	csjp::String str;
	str <<= ItemQueryTagOptions::MatchAll;
	VERIFY(str == "MatchAll");
	
	str <<= ItemQueryTagOptions::MatchAny;
	VERIFY(str == "MatchAny");
	
	TESTSTEP("Test ItemQueryTagOptions to QString");
	QString qstr;
	qstr <<= ItemQueryTagOptions::MatchAny;
	VERIFY(qstr == "MatchAny");
	
	TESTSTEP("Test QVariant to ItemQueryTagOptions");
	QVariant var("MatchAll");
	ItemQueryTagOptions opt = ItemQueryTagOptions::MatchAny;
	opt <<= var;
	VERIFY(opt == ItemQueryTagOptions::MatchAll);
	
	TESTSTEP("Test csjp::CString to ItemQueryTagOptions");
	ItemQueryTagOptions opt2 = ItemQueryTagOptions::MatchAll;
	opt2 <<= csjp::CString("MatchAny");
	VERIFY(opt2 == ItemQueryTagOptions::MatchAny);
}

void TestTypes::testStringConversions()
{
	TESTSTEP("Test bool to csjp::String");
	csjp::String str;
	str <<= true;
	VERIFY(str == "Y");
	
	str <<= false;
	VERIFY(str == "N");
	
	TESTSTEP("Test csjp::String self-assignment");
	csjp::String str1("hello");
	csjp::String str2("world");
	str1 <<= str2;
	VERIFY(str1 == "world");
	
	TESTSTEP("Test csjp::String assignment from char*");
	csjp::String str3;
	str3 <<= "test";
	VERIFY(str3 == "test");
}

TEST_INIT(Types)
	TEST_RUN(testSqlDriverEnum);
	TEST_RUN(testSqlDriverConversions);
	TEST_RUN(testYNBoolConversions);
	TEST_RUN(testItemQueryStockOptions);
	TEST_RUN(testItemQueryTagOptions);
	TEST_RUN(testStringConversions);
TEST_FINISH(Types)
