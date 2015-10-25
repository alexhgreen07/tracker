#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "app_db.hpp"

using std::make_shared;
using std::shared_ptr;

using namespace Tracker;
using namespace Application;
using namespace Core;

TEST_BASE(AppDBGroupBase)
{
    shared_ptr<Database::DatabaseSqlite3> mysqlDB;
    AppDB testDB;
    
    AppDBGroupBase() :
    	mysqlDB(make_shared<Database::DatabaseSqlite3>()),
        testDB(mysqlDB)
    {
    }

    shared_ptr<Task> insertDummyTask()
	{
    	auto newTask = make_shared<Task>();
    	uint64_t taskId = testDB.insertTask(*newTask);
    	newTask->setTaskId(taskId);

    	return newTask;
	}

    shared_ptr<Task> insertDummyRecurringTask()
	{
    	auto newTask = make_shared<Task>("",0,10,1);
    	newTask->setRecurranceParameters(1,0);
		uint64_t taskId = testDB.insertTask(*newTask);
		newTask->setTaskId(taskId);

		return newTask;
	}

    shared_ptr<Event> insertDummyEvent()
	{
    	auto newEvent = make_shared<Event>(1,2);
    	auto parent = insertDummyTask();
    	newEvent->setStatus(Event::Status::Running);
		newEvent->setParent(parent);
		newEvent->setEventId(testDB.insertEvent(*newEvent));
    	return newEvent;
	}

    shared_ptr<Event> updateDummyEvent(uint64_t eventId)
	{
    	auto updatedEvent = make_shared<Event>(2,3);
    	auto parent = insertDummyTask();
		parent->setTaskId(2);
		updatedEvent->setStatus(Event::Status::Logged);
		updatedEvent->setParent(parent);
		testDB.updateEvent(eventId,*updatedEvent);

    	return updatedEvent;
	}

    shared_ptr<Event> insertDummyRecurringEvent()
	{
    	auto newEvent = make_shared<Event>(1,2);
		auto parent = insertDummyRecurringTask();
		newEvent->setParent(parent->getRecurringChild(1));
		newEvent->setEventId(testDB.insertEvent(*newEvent));
		return newEvent;
	}

    shared_ptr<Event> updateRecurringDummyEvent(uint64_t eventId)
	{
		auto updatedEvent = make_shared<Event>(1,2);
		auto parent = insertDummyRecurringTask();
		updatedEvent->setParent(parent->getRecurringChild(2));
		testDB.updateEvent(eventId,*updatedEvent);

		return updatedEvent;
	}
};

TEST_GROUP_BASE(AppDBGroup, AppDBGroupBase)
{
    
    TEST_SETUP()
    {
        mysqlDB->open(":memory:");
        testDB.initializeNewDatabase();
    }
    
    TEST_TEARDOWN()
    {
        mysqlDB->close();
    }
};

TEST(AppDBGroup, ValidateVersionTable)
{
    auto result = mysqlDB->select("select version from version");
    
    STRCMP_EQUAL(result->at(0)[0].c_str(),testDB.getCurrentVersion().c_str());
}

TEST(AppDBGroup, ValidateTasksTableExists)
{
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(0, result->size());
}

TEST(AppDBGroup, ValidateTaskInsert)
{
    Task newTask;
    
    testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(1, result->size());
}

TEST(AppDBGroup, ValidateTaskInsertByTaskId)
{
	Task newTask;

	testDB.insertTask(newTask);

	auto result = testDB.getTasks();
	auto testTask = result->at(1);

	LONGS_EQUAL(1,testTask->getTaskId());
}

TEST(AppDBGroup, ValidateTaskInsertByName)
{
	Task newTask;

	newTask.setName("test task");
	unsigned int taskId = testDB.insertTask(newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	STRCMP_EQUAL(newTask.getName().c_str(),retrievedTask->getName().c_str());
}

TEST(AppDBGroup, ValidateTaskInsertByEarliestStartTime)
{
    Task newTask;
    
    newTask.setEarliestStartTime(2);
    unsigned int taskId = testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getEarliestStartTime(),retrievedTask->getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskInsertByLatestEndTime)
{
    Task newTask;
    
    newTask.setLatestEndTime(2);
    unsigned int taskId = testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getLatestEndTime(),retrievedTask->getLatestEndTime());
}

TEST(AppDBGroup, ValidateTaskInsertByDuration)
{
    Task newTask;
    
    newTask.setDuration(2);
    unsigned int taskId = testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getDuration(),retrievedTask->getDuration());
}

TEST(AppDBGroup, ValidateTaskInsertByStatus)
{
	Task newTask;

	newTask.setStatus(Task::Status::Complete);
	unsigned int taskId = testDB.insertTask(newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	CHECK(newTask.getStatus() == retrievedTask->getStatus());
}

TEST(AppDBGroup, ValidateTaskInsertByRecurringChildrenCount)
{
	auto newTask = make_shared<Task>("",0,50,5);

	newTask->setRecurranceParameters(10,5);
	unsigned int taskId = testDB.insertTask(*newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	LONGS_EQUAL(newTask->getRecurringTaskCount(), retrievedTask->getRecurringTaskCount());
}

TEST(AppDBGroup, ValidateTaskDelete)
{
    Task newTask;
    
    uint64_t taskId = testDB.insertTask(newTask);
    testDB.removeTask(taskId);
    
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(0, result->size());
}

TEST(AppDBGroup, ValidateTaskUpdateByName)
{
	Task newTask;

	auto taskId = testDB.insertTask(newTask);

	newTask.setName("test name");
	testDB.updateTask(taskId, newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	STRCMP_EQUAL(newTask.getName().c_str(),retrievedTask->getName().c_str());
}

TEST(AppDBGroup, ValidateTaskUpdateByEarliestStartTime)
{
    Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setEarliestStartTime(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getEarliestStartTime(),retrievedTask->getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskUpdateByLatestEndTime)
{
    Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setLatestEndTime(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getLatestEndTime(),retrievedTask->getLatestEndTime());
}

TEST(AppDBGroup, ValidateTaskUpdateByDuration)
{
    Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setDuration(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getDuration(),retrievedTask->getDuration());
}

TEST(AppDBGroup, ValidateTaskUpdateByStatus)
{
    Task newTask;

    auto taskId = testDB.insertTask(newTask);

    newTask.setStatus(Task::Status::Complete);
    testDB.updateTask(taskId, newTask);

    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    CHECK(newTask.getStatus() == retrievedTask->getStatus());
}

TEST(AppDBGroup, ValidateTaskUpdateByRecurringChildrenCount)
{
	auto newTask = make_shared<Task>("",0,50,5);

	newTask->setRecurranceParameters(10,5);
	unsigned int taskId = testDB.insertTask(*newTask);

	newTask->setRecurranceParameters(5,0);
	testDB.updateTask(taskId, *newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	LONGS_EQUAL(newTask->getRecurringTaskCount(), retrievedTask->getRecurringTaskCount());
}

TEST(AppDBGroup, ValidateRecurringTaskStatusUpdate)
{
	const unsigned int recurringChildIndex = 0;
	auto newTask = make_shared<Task>("",0,50,5);

	newTask->setRecurranceParameters(10,5);
	unsigned int taskId = testDB.insertTask(*newTask);

	testDB.updateRecurringTaskStatus(taskId,recurringChildIndex,Task::Status::Complete);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	auto recurringChild = retrievedTask->getRecurringChild(recurringChildIndex);
	CHECK(recurringChild->getStatus() == Task::Status::Complete);
}

TEST(AppDBGroup, ValidateEventsTableExists)
{
	auto result = testDB.getLoggedEvents();

	LONGS_EQUAL(0, result->size());
}


TEST(AppDBGroup, ValidateEventInsert)
{
	insertDummyEvent();

    auto result = testDB.getLoggedEvents();

    LONGS_EQUAL(1, result->size());
}

TEST(AppDBGroup, ValidateEventInsertByStartTime)
{
	auto newEvent = insertDummyEvent();

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	LONGS_EQUAL(newEvent->getStartTime(), insertedEvent->getStartTime());
}

TEST(AppDBGroup, ValidateEventInsertByDuration)
{
	auto newEvent = insertDummyEvent();

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	LONGS_EQUAL(newEvent->getDuration(), insertedEvent->getDuration());
}

TEST(AppDBGroup, ValidateEventInsertByEventId)
{
	auto newEvent = insertDummyEvent();

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	LONGS_EQUAL(newEvent->getEventId(), insertedEvent->getEventId());
}

TEST(AppDBGroup, ValidateEventInsertByStatus)
{
	auto newEvent = insertDummyEvent();

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	CHECK(Event::Status::Running == insertedEvent->getStatus());
}

TEST(AppDBGroup, ValidateEventInsertByParentTaskId)
{
	auto newEvent = insertDummyEvent();

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	LONGS_EQUAL(newEvent->getParent()->getTaskId(), insertedEvent->getParent()->getTaskId());
}

TEST(AppDBGroup, ValidateRecurringEventInsert)
{
	auto newEvent = insertDummyRecurringEvent();

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	LONGS_EQUAL(
			newEvent->getParent()->getRecurringIndex(),
			insertedEvent->getParent()->getRecurringIndex());
}

TEST(AppDBGroup, ValidateEventDelete)
{
	auto newEvent = insertDummyEvent();

    testDB.removeEvent(newEvent->getEventId());

    auto result = testDB.getLoggedEvents();

    LONGS_EQUAL(0, result->size());
}

TEST(AppDBGroup, ValidateEventUpdateByStartTime)
{
	auto newEvent = insertDummyEvent();
	auto updatedEvent = updateDummyEvent(newEvent->getEventId());

	auto result = testDB.getLoggedEvents();
	auto updatedDbEvent = result->at(newEvent->getEventId());
    LONGS_EQUAL(updatedEvent->getStartTime(),updatedDbEvent->getStartTime());
}

TEST(AppDBGroup, ValidateEventUpdateByDuration)
{
	auto newEvent = insertDummyEvent();
	auto updatedEvent = updateDummyEvent(newEvent->getEventId());

	auto result = testDB.getLoggedEvents();
	auto updatedDbEvent = result->at(newEvent->getEventId());
    LONGS_EQUAL(updatedEvent->getDuration(),updatedDbEvent->getDuration());
}

TEST(AppDBGroup, ValidateEventUpdateByStatus)
{
	auto newEvent = insertDummyEvent();
	auto updatedEvent = updateDummyEvent(newEvent->getEventId());

	auto result = testDB.getLoggedEvents();
	auto updatedDbEvent = result->at(newEvent->getEventId());
    CHECK(Event::Status::Logged == updatedDbEvent->getStatus());
}

TEST(AppDBGroup, ValidateEventUpdateByParentTaskId)
{
	auto newEvent = insertDummyEvent();
	auto updatedEvent = updateDummyEvent(newEvent->getEventId());

	auto result = testDB.getLoggedEvents();
	auto updatedDbEvent = result->at(newEvent->getEventId());
    LONGS_EQUAL(updatedEvent->getParent()->getTaskId(),updatedDbEvent->getParent()->getTaskId());
}

TEST(AppDBGroup, ValidateRecurringEventUpdate)
{
	auto newEvent = insertDummyRecurringEvent();
	auto updatedEvent = updateRecurringDummyEvent(newEvent->getEventId());

	auto result = testDB.getLoggedEvents();

	auto insertedEvent = result->at(newEvent->getEventId());
	LONGS_EQUAL(
			updatedEvent->getParent()->getRecurringIndex(),
			insertedEvent->getParent()->getRecurringIndex());
}

