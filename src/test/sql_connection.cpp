/*
 * Author: Butler Test Suite
 * Unit tests for butler_sql_connection.cpp
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_sql_connection.h>

class TestSqlConnection
{
public:
	void testDatabaseDescriptorConstruction();
	void testDatabaseDescriptorCopy();
	void testDatabaseDescriptorMove();
	void testDatabaseDescriptorComparison();
	void testDatabaseDescriptorToJson();
	void testDatabaseDescriptorSet();
	void testSqlConnectionCreation();
};

void TestSqlConnection::testDatabaseDescriptorConstruction()
{
	TESTSTEP("Test DatabaseDescriptor default construction");
	DatabaseDescriptor desc1;
	VERIFY(desc1.name.length == 0);
	VERIFY(desc1.databaseName.length == 0);
	
	TESTSTEP("Test DatabaseDescriptor construction with name");
	csjp::String name("TestDB");
	DatabaseDescriptor desc2(name);
	VERIFY(desc2.name == name);
}

void TestSqlConnection::testDatabaseDescriptorCopy()
{
	TESTSTEP("Test DatabaseDescriptor copy construction");
	DatabaseDescriptor orig;
	orig.name = "TestDB";
	orig.driver = SqlDriver::Enum::SQLite;
	orig.databaseName = "test.db";
	orig.username = "user";
	orig.password = "pass";
	orig.savePassword.val = true;
	orig.host = "localhost";
	orig.port = 5432;
	
	DatabaseDescriptor copy(orig);
	VERIFY(copy.name == orig.name);
	VERIFY(copy.driver == orig.driver);
	VERIFY(copy.databaseName == orig.databaseName);
	VERIFY(copy.username == orig.username);
	VERIFY(copy.password == orig.password);
	VERIFY(copy.savePassword == orig.savePassword);
	VERIFY(copy.host == orig.host);
	VERIFY(copy.port == orig.port);
	
	TESTSTEP("Test DatabaseDescriptor copy assignment");
	DatabaseDescriptor assigned;
	assigned = orig;
	VERIFY(assigned.name == orig.name);
	VERIFY(assigned.databaseName == orig.databaseName);
}

void TestSqlConnection::testDatabaseDescriptorMove()
{
	TESTSTEP("Test DatabaseDescriptor move construction");
	DatabaseDescriptor orig;
	orig.name = "MoveDB";
	orig.databaseName = "move.db";
	
	DatabaseDescriptor moved(csjp::move_cast(orig));
	VERIFY(moved.name == "MoveDB");
	VERIFY(moved.databaseName == "move.db");
	
	TESTSTEP("Test DatabaseDescriptor move assignment");
	DatabaseDescriptor dest;
	DatabaseDescriptor src;
	src.name = "AssignDB";
	src.databaseName = "assign.db";
	
	dest = csjp::move_cast(src);
	VERIFY(dest.name == "AssignDB");
	VERIFY(dest.databaseName == "assign.db");
}

void TestSqlConnection::testDatabaseDescriptorComparison()
{
	TESTSTEP("Test DatabaseDescriptor equality");
	DatabaseDescriptor desc1;
	desc1.name = "DB1";
	desc1.driver = SqlDriver::Enum::SQLite;
	
	DatabaseDescriptor desc2;
	desc2.name = "DB1";
	desc2.driver = SqlDriver::Enum::SQLite;
	
	VERIFY(desc1.isEqual(desc2));
	VERIFY(desc1 == desc2);
	
	TESTSTEP("Test DatabaseDescriptor inequality");
	desc2.name = "DB2";
	VERIFY(!desc1.isEqual(desc2));
	VERIFY(desc1 != desc2);
	
	TESTSTEP("Test DatabaseDescriptor comparison");
	VERIFY(desc1.isLess(desc2));
	VERIFY(desc1 < desc2);
	VERIFY(desc2.isMore(desc1));
}

void TestSqlConnection::testDatabaseDescriptorToJson()
{
	TESTSTEP("Test DatabaseDescriptor toJson");
	DatabaseDescriptor desc;
	desc.name = "JsonDB";
	desc.driver = SqlDriver::Enum::SQLite;
	desc.databaseName = "json.db";
	desc.username = "testuser";
	desc.port = 5432;
	
	csjp::Json json = desc.toJson();
	VERIFY(json.has("name"));
	VERIFY(json.has("driver"));
	VERIFY(json.has("databaseName"));
	VERIFY(json.has("username"));
	VERIFY(json.has("port"));
}

void TestSqlConnection::testDatabaseDescriptorSet()
{
	TESTSTEP("Test DatabaseDescriptorSet creation");
	DatabaseDescriptorSet set;
	VERIFY(set.size() == 0);
	VERIFY(set.ascending == true);
	
	TESTSTEP("Test DatabaseDescriptorSet add and sort");
	DatabaseDescriptor *desc1 = new DatabaseDescriptor();
	desc1->name = "DB_C";
	DatabaseDescriptor *desc2 = new DatabaseDescriptor();
	desc2->name = "DB_A";
	DatabaseDescriptor *desc3 = new DatabaseDescriptor();
	desc3->name = "DB_B";
	
	set.add(desc1);
	set.add(desc2);
	set.add(desc3);
	
	VERIFY(set.size() == 3);
	
	// Should be sorted by name in ascending order
	VERIFY(set[0].name == "DB_A");
	VERIFY(set[1].name == "DB_B");
	VERIFY(set[2].name == "DB_C");
}

void TestSqlConnection::testSqlConnectionCreation()
{
	TESTSTEP("Test SqlConnection creation with valid descriptor");
	DatabaseDescriptor desc;
	desc.name = "TestConnection";
	desc.driver = SqlDriver::Enum::SQLite;
	desc.databaseName = "testdir/test.db";
	
	SqlConnection sql(desc);
	VERIFY(sql.desc.name == desc.name);
	VERIFY(sql.desc.driver == desc.driver);
	
	TESTSTEP("Test SqlConnection is initially closed");
	// Note: We don't test open() here as it requires actual database setup
	// That would be an integration test rather than a unit test
}

TEST_INIT(SqlConnection)
	TEST_RUN(testDatabaseDescriptorConstruction);
	TEST_RUN(testDatabaseDescriptorCopy);
	TEST_RUN(testDatabaseDescriptorMove);
	TEST_RUN(testDatabaseDescriptorComparison);
	TEST_RUN(testDatabaseDescriptorToJson);
	TEST_RUN(testDatabaseDescriptorSet);
	TEST_RUN(testSqlConnectionCreation);
TEST_FINISH(SqlConnection)
