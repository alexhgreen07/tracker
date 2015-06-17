#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "app_api.hpp"

using namespace Tracker;
using namespace Application;

TEST_BASE(AppApiGroupBase)
{
	AppApi testApi;
	
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

TEST(AppApiGroup, ValidateSayHello)
{
	auto procedures = testApi.getProcedures();
	Json::Value params;
	Json::Value results;
	
	params["name"] = "test name";
	procedures["sayHello"]->call(params,results);
	
	Json::Value desiredResult = "Hello: " + params["name"].asString();
	CHECK(results == desiredResult);
}