#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

TEST_BASE(AppApiGroupBase)
{
	AppApiGroupBase()
	{}
};

TEST_GROUP_BASE(AppApiGroup, AppApiGroupBase)
{
	
	TEST_SETUP()
	{
	}
	
	TEST_TEARDOWN()
	{
	}
};

TEST(AppApiGroup, Basic)
{
}