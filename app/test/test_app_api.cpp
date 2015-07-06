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
	
	Json::Value desiredResult = Json::Value(Json::arrayValue);
	CHECK(results == desiredResult);
}

TEST(AppApiGroup, ValidateGetTaskTableWithSingleEntry)
{
	Core::Task newTask("",2,3,4);
	unsigned int expectedIndex = 0;
	db.insertTask(newTask);
	
	procedures["getTasks"]->call(params,results);
	
	LONGS_EQUAL(1,results.size());
	
	LONGS_EQUAL(1,results[expectedIndex]["taskId"].asInt());
	STRCMP_EQUAL(newTask.getName().c_str(),results[expectedIndex]["name"].asCString());
	LONGS_EQUAL(newTask.getEarliestStartTime(),results[expectedIndex]["earliestStartTime"].asInt());
	LONGS_EQUAL(newTask.getLatestEndTime(),results[expectedIndex]["latestEndTime"].asInt());
	LONGS_EQUAL(newTask.getDuration(),results[expectedIndex]["duration"].asInt());
}

TEST(AppApiGroup, ValidateGetTaskTableWithMultipleEntries)
{
	constexpr unsigned int loopLimit = 5;
	
	for(unsigned int i = 0; i < loopLimit; i++)
	{
		Core::Task newTask(std::to_string(i),i,i+1,i+2);
		db.insertTask(newTask);
	}

	procedures["getTasks"]->call(params,results);
	
	LONGS_EQUAL(loopLimit,results.size());
	
	for(unsigned int i = 0; i < loopLimit; i++)
	{
		LONGS_EQUAL(i + 1,results[i]["taskId"].asInt());
		STRCMP_EQUAL(std::to_string(i).c_str(),results[i]["name"].asCString());
		LONGS_EQUAL((i),results[i]["earliestStartTime"].asInt());
		LONGS_EQUAL((i + 1),results[i]["latestEndTime"].asInt());
		LONGS_EQUAL((i + 2),results[i]["duration"].asInt());
	}
}

TEST(AppApiGroup, ValidateGetTaskTableWithRecurringEntries)
{
	auto newTask = std::make_shared<Core::Task>("",0,50,5);
	newTask->setRecurranceParameters(10,0);
	unsigned int parentTaskId = db.insertTask(*newTask);

	procedures["getTasks"]->call(params,results);

	LONGS_EQUAL(newTask->getRecurringTaskCount() + 1,results.size());

	for(unsigned int i = 0; i < results.size(); i++)
	{
		if(results[i]["taskId"].asInt() != parentTaskId)
		{
			LONGS_EQUAL(results[i]["recurringParentTaskId"].asInt(),parentTaskId);
		}
	}
}

TEST(AppApiGroup, InsertTask)
{
	std::string testName = "test name";
	params["name"] = testName;
	params["earliestStartTime"] = 1;
	params["latestEndTime"] = 2;
	params["duration"] = 3;
	
	procedures["insertTask"]->call(params,results);
	
	auto result = db.getTasks();
	
	LONGS_EQUAL(1,result->size());
	
	auto task = result->at(1);
	STRCMP_EQUAL(testName.c_str(),task->getName().c_str());
	LONGS_EQUAL(1,task->getEarliestStartTime());
	LONGS_EQUAL(2,task->getLatestEndTime());
	LONGS_EQUAL(3,task->getDuration());
}

TEST(AppApiGroup, UpdateTask)
{
	std::string testName = "test name";
	Core::Task newTask("",1,2,1);
	db.insertTask(newTask);
	
	params["taskId"] = 1;
	params["name"] = testName;
	params["earliestStartTime"] = 2;
	params["latestEndTime"] = 4;
	params["duration"] = 2;
	
	procedures["updateTask"]->call(params,results);
	
	auto result = db.getTasks();
	
	auto task = result->at(1);
	
	STRCMP_EQUAL(testName.c_str(),task->getName().c_str());
	LONGS_EQUAL(2,task->getEarliestStartTime());
	LONGS_EQUAL(4,task->getLatestEndTime());
	LONGS_EQUAL(2,task->getDuration());
}

TEST(AppApiGroup, RemoveTask)
{
	Core::Task newTask("",1,1,1);
	db.insertTask(newTask);
	
	params["taskId"] = 1;
	procedures["removeTask"]->call(params,results);
	
	auto result = db.getTasks();
	
	LONGS_EQUAL(0,result->size());
}

TEST(AppApiGroup, GetEvents)
{
	Core::Task newTask("",0,1,1);
	unsigned int expectedIndex = 0;

	newTask.setName("test name");

	db.insertTask(newTask);
	
	procedures["getEvents"]->call(params,results);
	
	LONGS_EQUAL(1,results.size());
	
	LONGS_EQUAL(1,results[expectedIndex]["taskId"].asInt());
	STRCMP_EQUAL(newTask.getName().c_str(),results[expectedIndex]["name"].asCString());
	LONGS_EQUAL(newTask.getEarliestStartTime(),results[expectedIndex]["startTime"].asInt());
	LONGS_EQUAL(newTask.getDuration(),results[expectedIndex]["duration"].asInt());
}

