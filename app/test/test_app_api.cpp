#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "app_api.hpp"

using namespace Tracker;
using namespace Application;

TEST_BASE(AppApiGroupBase)
{
	Database::DatabaseSqlite3 sqlDB;
	AppDB db;
	AppApi testApi;
	JsonMethods & procedures;
	
	AppApiGroupBase() :
		db(sqlDB),
		testApi(db),
		procedures(testApi.getProcedures())
	{}
};

TEST_GROUP_BASE(AppApiGroup, AppApiGroupBase)
{
	Json::Value params;
	Json::Value results;
	
	TEST_SETUP()
	{
		sqlDB.open(":memory:");
		db.initializeNewDatabase();
	}
	
	TEST_TEARDOWN()
	{
		sqlDB.close();
	}
};

TEST(AppApiGroup, ValidateSayHello)
{
	params["name"] = "test name";
	procedures["sayHello"]->call(params,results);
	
	Json::Value desiredResult = "Hello: " + params["name"].asString();
	CHECK(results == desiredResult);
}

TEST(AppApiGroup, ValidateGetEmptyTaskTable)
{
	procedures["getTasks"]->call(params,results);
	
	Json::Value desiredResult;
	CHECK(results == desiredResult);
}

TEST(AppApiGroup, ValidateGetTaskTableWithSingleEntry)
{
	Json::Value desiredResult;
	Core::Task newTask;
	newTask.setEarliestStartTime(2);
	newTask.setLatestEndTime(3);
	newTask.setDuration(4);
	db.insertTask(newTask);
	
	procedures["getTasks"]->call(params,results);
	
	CHECK(results[1]["taskId"].asInt() == 1);
	CHECK(results[1]["earliestStartTime"].asInt() == newTask.getEarliestStartTime());
	CHECK(results[1]["latestEndTime"].asInt() == newTask.getLatestEndTime());
	CHECK(results[1]["duration"].asInt() == newTask.getDuration());
}

