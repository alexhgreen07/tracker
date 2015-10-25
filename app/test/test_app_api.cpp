#include <string>
#include <iostream>
#include <sstream>

#include <CppUTest/TestHarness.h>

#include "app_api.hpp"

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::istringstream;
using std::to_string;

using namespace Tracker;
using namespace Application;
using namespace Core;

class TestClock : public AppApi::Clock
{
	uint64_t getNowTimestamp() override
	{
		return 0;
	}
};

TEST_BASE(AppApiGroupBase)
{
	shared_ptr<Database::DatabaseSqlite3> sqlDB;
	shared_ptr<AppDB> db;
	shared_ptr<TestClock> testClock;
	AppApi testApi;
	JsonMethods & procedures;
	
	AppApiGroupBase() :
		sqlDB(make_shared<Database::DatabaseSqlite3>()),
		db(make_shared<AppDB>(sqlDB)),
		testClock(make_shared<TestClock>()),
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
	auto newTask = make_shared<Task>("",2,3,4);
	newTask->setStatus(Task::Status::Complete);
	newTask->setRecurranceParameters(10,1);
	unsigned int expectedIndex = 0;
	
	unsigned int taskId = db->insertTask(*newTask);

	procedures["getTasks"]->call(params,results);
	
	LONGS_EQUAL(1,results.size());
	
	LONGS_EQUAL(taskId,results[expectedIndex]["taskId"].asInt());
	STRCMP_EQUAL(newTask->getName().c_str(),results[expectedIndex]["name"].asCString());

	istringstream input_stream(results[expectedIndex]["earliestStartTime"].asString());
	uint64_t value;

	input_stream >> value;
	LONGS_EQUAL(newTask->getEarliestStartTime(),value);

	input_stream = istringstream(results[expectedIndex]["latestEndTime"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getLatestEndTime(),value);

	input_stream = istringstream(results[expectedIndex]["duration"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getDuration(),value);

	STRCMP_EQUAL("Complete",results[expectedIndex]["status"].asCString());

	input_stream = istringstream(results[expectedIndex]["recurringPeriod"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getRecurringPeriod(),value);

	input_stream = istringstream(results[expectedIndex]["recurringLateOffset"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getRecurringLateOffset(),value);

	LONGS_EQUAL(newTask->getRecurringTaskCount(),results[expectedIndex]["recurringCount"].asInt());
}

TEST(AppApiGroup, ValidateGetTaskTableWithMultipleEntries)
{
	constexpr unsigned int loopLimit = 5;
	
	for(unsigned int i = 0; i < loopLimit; i++)
	{
		Task newTask(to_string(i),i,i+1,i+2);
		db->insertTask(newTask);
	}

	procedures["getTasks"]->call(params,results);
	
	LONGS_EQUAL(loopLimit,results.size());
	
	for(unsigned int i = 0; i < loopLimit; i++)
	{
		LONGS_EQUAL(i + 1,results[i]["taskId"].asInt());
		STRCMP_EQUAL(to_string(i).c_str(),results[i]["name"].asCString());
		
		istringstream input_stream(results[i]["earliestStartTime"].asString());
		uint64_t value;

		input_stream >> value;
		LONGS_EQUAL((i),value);
		
		input_stream = istringstream(results[i]["latestEndTime"].asString());
		input_stream >> value;
		LONGS_EQUAL((i + 1),value);

		input_stream = istringstream(results[i]["duration"].asString());
		input_stream >> value;
		LONGS_EQUAL((i + 2),value);
	}
}

TEST(AppApiGroup, InsertTask)
{
	string testName = "test name";
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
	CHECK(task->getStatus() == Task::Status::Complete);
}

TEST(AppApiGroup, InsertRecurringTask)
{
	string testName = "test name";
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
	string testName = "test name";
	Task newTask("",1,2,1);
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
	CHECK(task->getStatus() == Task::Status::Complete);
}

TEST(AppApiGroup, UpdateRecurringTask)
{
	auto newTask = make_shared<Task>("",0,50,5);
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

TEST(AppApiGroup, UpdateRecurringTaskStatus)
{
	const unsigned int recurringIndex = 0;
	const Task::Status testStatus = Task::Status::Complete;

	auto newTask = make_shared<Task>("",0,50,5);
	newTask->setRecurranceParameters(10,0);
	unsigned int parentTaskId = db->insertTask(*newTask);

	params["taskId"] = to_string(parentTaskId);
	params["recurringIndex"] = to_string(recurringIndex);
	params["status"] = to_string((unsigned int)testStatus);

	procedures["updateRecurringTaskStatus"]->call(params,results);

	auto result = db->getTasks();

	auto task = result->at(parentTaskId);

	auto recurringChild = task->getRecurringChild(recurringIndex);

	CHECK(recurringChild->getStatus() == testStatus);
}

TEST(AppApiGroup, RemoveTask)
{
	Task newTask("",1,1,1);
	db->insertTask(newTask);
	
	params["taskId"] = 1;
	procedures["removeTask"]->call(params,results);
	
	auto result = db->getTasks();
	
	LONGS_EQUAL(0,result->size());
}

TEST(AppApiGroup, GetEvents)
{
	auto newTask = make_shared<Task>("",0,1,1);
	unsigned int expectedIndex = 0;

	newTask->setName("test name");

	db->insertTask(*newTask);
	
	procedures["getEvents"]->call(params,results);
	
	LONGS_EQUAL(1,results.size());
	
	uint64_t value;

	istringstream input_stream(results[expectedIndex]["eventId"].asString());
	input_stream >> value;
	LONGS_EQUAL(0,value);

	input_stream = istringstream(results[expectedIndex]["taskId"].asString());
	input_stream >> value;
	LONGS_EQUAL(1,value);

	STRCMP_EQUAL("Scheduled",results[expectedIndex]["status"].asCString());

	input_stream = istringstream(results[expectedIndex]["startTime"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getEarliestStartTime(),value);

	input_stream = istringstream(results[expectedIndex]["duration"].asString());
	input_stream >> value;
	LONGS_EQUAL(newTask->getDuration(),value);

	LONGS_EQUAL(newTask->getRecurringIndex(),results[expectedIndex]["recurringIndex"].asInt());
}


TEST(AppApiGroup, InsertEvent)
{
	Task newTask("test task",1,1,1);
	newTask.setStatus(Task::Status::Complete);
	uint64_t taskId = db->insertTask(newTask);

	params["startTime"] = "2";
	params["duration"] = "3";
	params["taskId"] = to_string(taskId);
	params["status"] = "Running";

	procedures["insertEvent"]->call(params,results);

	auto result = db->getLoggedEvents();

	LONGS_EQUAL(1,result->size());

	auto event = result->at(1);
	LONGS_EQUAL(2,event->getStartTime());
	LONGS_EQUAL(3,event->getDuration());
	LONGS_EQUAL(taskId,event->getParent()->getTaskId());
	CHECK(event->getStatus() == Event::Status::Running);
}

TEST(AppApiGroup, UpdateEvent)
{
	auto newTask = make_shared<Task>("test task",1,1,1);
	newTask->setStatus(Task::Status::Complete);
	newTask->setTaskId(db->insertTask(*newTask));
	auto newEvent = make_shared<Event>(1,2);
	newEvent->setParent(newTask);
	newEvent->setEventId(db->insertEvent(*newEvent));

	auto updatedParent = make_shared<Task>("test task 2",1,1,1);
	updatedParent->setStatus(Task::Status::Complete);
	updatedParent->setTaskId(db->insertTask(*updatedParent));

	params["eventId"] = to_string(newEvent->getEventId());
	params["startTime"] = "2";
	params["duration"] = "3";
	params["taskId"] = to_string(updatedParent->getTaskId());
	params["status"] = "Running";

	procedures["updateEvent"]->call(params,results);

	auto result = db->getLoggedEvents();
	auto event = result->at(newEvent->getEventId());
	LONGS_EQUAL(2,event->getStartTime());
	LONGS_EQUAL(3,event->getDuration());
	LONGS_EQUAL(updatedParent->getTaskId(),event->getParent()->getTaskId());
	LONGS_EQUAL(Event::Status::Running,event->getStatus());
}

TEST(AppApiGroup, RemoveEvent)
{
	auto newTask = make_shared<Task>("test task",1,1,1);
	newTask->setStatus(Task::Status::Complete);
	newTask->setTaskId(db->insertTask(*newTask));
	auto newEvent = make_shared<Event>(1,2);
	newEvent->setParent(newTask);
	newEvent->setEventId(db->insertEvent(*newEvent));

	params["eventId"] = to_string(newEvent->getEventId());

	procedures["removeEvent"]->call(params,results);

	auto result = db->getLoggedEvents();

	LONGS_EQUAL(0,result->size());
}

TEST(AppApiGroup, InsertRecurringEvent)
{
	auto newTask = make_shared<Task>("test task",0,10,1);
	newTask->setRecurranceParameters(1,0);
	newTask->setStatus(Task::Status::Complete);
	uint64_t taskId = db->insertTask(*newTask);

	params["startTime"] = "2";
	params["duration"] = "3";
	params["taskId"] = to_string(taskId);
	params["recurringIndex"] = "1";

	procedures["insertEvent"]->call(params,results);

	auto tasks = db->getTasks();
	auto result = db->getLoggedEvents();

	LONGS_EQUAL(1,result->size());

	auto event = result->at(1);
	LONGS_EQUAL(1,event->getParent()->getRecurringIndex());
}

TEST(AppApiGroup, UpdateRecurringEvent)
{
	auto newTask = make_shared<Task>("test task",0,10,1);
	newTask->setRecurranceParameters(1,0);
	newTask->setStatus(Task::Status::Complete);
	uint64_t taskId = db->insertTask(*newTask);
	auto newEvent = make_shared<Event>(1,2);
	newEvent->setParent(newTask->getRecurringChild(1));
	newEvent->setEventId(db->insertEvent(*newEvent));

	params["eventId"] = to_string(newEvent->getEventId());
	params["startTime"] = "2";
	params["duration"] = "3";
	params["taskId"] = to_string(taskId);
	params["recurringIndex"] = "2";

	procedures["updateEvent"]->call(params,results);

	auto tasks = db->getTasks();
	auto result = db->getLoggedEvents();

	LONGS_EQUAL(1,result->size());

	auto event = result->at(1);
	LONGS_EQUAL(2,event->getParent()->getRecurringIndex());
}

