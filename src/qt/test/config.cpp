/*
 * Author: Butler Test Suite
 * Unit tests for butler_config.cpp
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_config.h>

class TestConfig
{
public:
	void testConfigNamespace();
};

void TestConfig::testConfigNamespace()
{
	TESTSTEP("Test Config namespace variables initialization");
	// Just verify the config namespace exists and can be used
	// We don't want to test Qt itself, just that our code compiles and links
	VERIFY(Config::scaleFactor >= 0.0);
	VERIFY(Config::pxPerMM >= 0.0);
}

TEST_INIT(Config)
	TEST_RUN(testConfigNamespace);
TEST_FINISH(Config)
