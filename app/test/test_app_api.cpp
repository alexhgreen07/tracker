#include <string>
#include <iostream>
#include <sstream>

#include <CppUTest/TestHarness.h>

#include "app_api.hpp"

using namespace Tracker;
using namespace Application;

class TestClock : public AppApi::Clock
{
	uint64_t getNowTimestamp() override
	{
		return 0;
	}
};

TEST_BASE(AppApiGroupBase)
{
	std::shared_ptr<Database::DatabaseSqlite3> sqlDB;
	std::shared_ptr<AppDB> db;
	std::shared_ptr<TestClock> testClock;
	AppApi testApi;
	JsonMethods & procedures;
	
	AppApiGroupBase() :
		sqlDB(std::make_shared<Database::DatabaseSqlite3>()),
		db(std::make_shared<AppDB>(sqlDB)),
		testClock(std::make_shared<TestClock>()),
		testApi(db,testClock),
		procedures(testApi.getProcedures())
	{}
};

TEST_GROUP_BASE(AppApiGroup, AppApiGroupBase)
{
	Json::Value params;
	Json::Value results;
	
	TEST_SETUP()
	{
		sqlDB->open(":memory:");
		db->initializeNewDatabase();
	}
	
	TEST_TEARDOWN()
	{
		sqlDB->close();
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
	auto newTask = std::make_shared<Core::Task>("",2,3,4);
	newTask->setStatus(Core::Task::Status::Complete);
	newTask->setRecurranceParameters(10,1);
	unsigned int expectedIndex = 0;
	
	unsigned int taskId = db->insertTask(*newTask);

	procedures["getTasks"]->call(params,results);
	
	LONGS_EQUAL(1,results.size());
	
	LONGS_EQUAL(taskId,results[expectedIndex]["taskId"].asInt());
	STRCMP_EQUAL(newTask->getName().c_str(),results[expectedIndex]["name"].asCString());

	std::istringstream input_stream(results[expectedIndex]["earliestStartTime"].asString());
	uint64_t value;

	input_stream >> value;
	LONGS_EQUAL(newTask->getEarliestStartTime(),value);

	input_stream = std::istringstream(results[expectedIndex]["latestEndTime"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getLatestEndTime(),value);

	input_stream = std::istringstream(results[expectedIndex]["duration"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getDuration(),value);

	STRCMP_EQUAL("Complete",results[expectedIndex]["status"].asCString());

	input_stream = std::istringstream(results[expectedIndex]["recurringPeriod"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getRecurringPeriod(),value);

	input_stream = std::istringstream(results[expectedIndex]["recurringLateOffset"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getRecurringLateOffset(),value);
}

TEST(AppApiGroup, ValidateGetTaskTableWithMultipleEntries)
{
	constexpr unsigned int loopLimit = 5;
	
	for(unsigned int i = 0; i < loopLimit; i++)
	{
		Core::Task newTask(std::to_string(i),i,i+1,i+2);
		db->insertTask(newTask);
	}

	procedures["getTasks"]->call(params,results);
	
	LONGS_EQUAL(loopLimit,results.size());
	
	for(unsigned int i = 0; i < loopLimit; i++)
	{
		LONGS_EQUAL(i + 1,results[i]["taskId"].asInt());
		STRCMP_EQUAL(std::to_string(i).c_str(),results[i]["name"].asCString());
		
		std::istringstream input_stream(results[i]["earliestStartTime"].asString());
		uint64_t value;

		input_stream >> value;
		LONGS_EQUAL((i),value);
		
		input_stream = std::istringstream(results[i]["latestEndTime"].asString());
		input_stream >> value;
		LONGS_EQUAL((i + 1),value);

		input_stream = std::istringstream(results[i]["duration"].asString());
		input_stream >> value;
		LONGS_EQUAL((i + 2),value);
	}
}

TEST(AppApiGroup, InsertTask)
{
	std::string testName = "test name";
	params["name"] = testName;
	params["earliestStartTime"] = "1";
	params["latestEndTime"] = "2";
	params["duration"] = "3";
	params["status"] = "Complete";
	
	procedures["insertTask"]->call(params,results);
	
	auto result = db->getTasks();
	
	LONGS_EQUAL(1,result->size());
	
	auto task = result->at(1);
	STRCMP_EQUAL(testName.c_str(),task->getName().c_str());
	LONGS_EQUAL(1,task->getEarliestStartTime());
	LONGS_EQUAL(2,task->getLatestEndTime());
	LONGS_EQUAL(3,task->getDuration());
	CHECK(task->getStatus() == Core::Task::Status::Complete);
}

TEST(AppApiGroup, InsertRecurringTask)
{
	std::string testName = "test name";
	params["name"] = testName;
	params["earliestStartTime"] = "0";
	params["latestEndTime"] = "50";
	params["duration"] = "5";

	params["recurringPeriod"] = "10";
	params["recurringLateOffset"] = "0";

	procedures["insertTask"]->call(params,results);

	auto result = db->getTasks();

	auto task = result->at(1);
	LONGS_EQUAL(5,task->getRecurringTaskCount());
	LONGS_EQUAL(10,task->getRecurringPeriod());
	LONGS_EQUAL(0,task->getRecurringLateOffset());
}

TEST(AppApiGroup, UpdateTask)
{
	std::string testName = "test name";
	Core::Task newTask("",1,2,1);
	unsigned int parentTaskId = db->insertTask(newTask);
	
	params["taskId"] = parentTaskId;
	params["name"] = testName;
	params["earliestStartTime"] = "2";
	params["latestEndTime"] = "4";
	params["duration"] = "2";
	params["status"] = "Complete";
	
	procedures["updateTask"]->call(params,results);
	
	auto result = db->getTasks();
	
	auto task = result->at(parentTaskId);
	
	STRCMP_EQUAL(testName.c_str(),task->getName().c_str());
	LONGS_EQUAL(2,task->getEarliestStartTime());
	LONGS_EQUAL(4,task->getLatestEndTime());
	LONGS_EQUAL(2,task->getDuration());
	CHECK(task->getStatus() == Core::Task::Status::Complete);
}

TEST(AppApiGroup, UpdateRecurringTask)
{
	auto newTask = std::make_shared<Core::Task>("",0,50,5);
	newTask->setRecurranceParameters(10,0);
	unsigned int parentTaskId = db->insertTask(*newTask);

	params["taskId"] = parentTaskId;
	params["name"] = "";
	params["earliestStartTime"] = "0";
	params["latestEndTime"] = "40";
	params["duration"] = "5";

	params["recurringPeriod"] = "5";
	params["recurringLateOffset"] = "0";

	procedures["updateTask"]->call(params,results);

	auto result = db->getTasks();

	auto task = result->at(parentTaskId);

	LONGS_EQUAL(8,task->getRecurringTaskCount());
	LONGS_EQUAL(5,task->getRecurringPeriod());
	LONGS_EQUAL(0,task->getRecurringLateOffset());
}

TEST(AppApiGroup, RemoveTask)
{
	Core::Task newTask("",1,1,1);
	db->insertTask(newTask);
	
	params["taskId"] = 1;
	procedures["removeTask"]->call(params,results);
	
	auto result = db->getTasks();
	
	LONGS_EQUAL(0,result->size());
}

TEST(AppApiGroup, GetEvents)
{
	Core::Task newTask("",0,1,1);
	unsigned int expectedIndex = 0;

	newTask.setName("test name");

	db->insertTask(newTask);
	
	procedures["getEvents"]->call(params,results);
	
	LONGS_EQUAL(1,results.size());
	
	LONGS_EQUAL(1,results[expectedIndex]["taskId"].asInt());
	STRCMP_EQUAL(newTask.getName().c_str(),results[expectedIndex]["name"].asCString());
	LONGS_EQUAL(newTask.getEarliestStartTime(),results[expectedIndex]["startTime"].asInt());
	LONGS_EQUAL(newTask.getDuration(),results[expectedIndex]["duration"].asInt());
}


TEST(AppApiGroup, InsertEvent)
{
	Core::Task newTask("test task",1,1,1);
	newTask.setStatus(Core::Task::Status::Complete);
	uint64_t taskId = db->insertTask(newTask);

	params["startTime"] = "2";
	params["duration"] = "3";
	params["taskId"] = std::to_string(taskId);

	procedures["insertEvent"]->call(params,results);

	auto result = db->getLoggedEvents();

	LONGS_EQUAL(1,result->size());

	auto event = result->at(1);
	LONGS_EQUAL(2,event->getStartTime());
	LONGS_EQUAL(3,event->getDuration());
	LONGS_EQUAL(taskId,event->getParent()->getTaskId());
}

