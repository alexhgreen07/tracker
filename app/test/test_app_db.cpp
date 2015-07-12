#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "app_db.hpp"

using namespace Tracker;
using namespace Application;

TEST_BASE(AppDBGroupBase)
{
    Database::DatabaseSqlite3 mysqlDB;
    AppDB testDB;
    
    AppDBGroupBase() :
        testDB(mysqlDB)
    {}
};

TEST_GROUP_BASE(AppDBGroup, AppDBGroupBase)
{
    
    TEST_SETUP()
    {
        mysqlDB.open(":memory:");
        testDB.initializeNewDatabase();
    }
    
    TEST_TEARDOWN()
    {
        mysqlDB.close();
    }
};

TEST(AppDBGroup, ValidateVersionTable)
{
    auto result = mysqlDB.select("select version from version");
    
    STRCMP_EQUAL(result->at(0)[0].c_str(),testDB.getCurrentVersion().c_str());
}

TEST(AppDBGroup, ValidateTasksTableExists)
{
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(0, result->size());
}

TEST(AppDBGroup, ValidateTaskInsert)
{
    Core::Task newTask;
    
    testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(1, result->size());
}

TEST(AppDBGroup, ValidateTaskInsertByTaskId)
{
	Core::Task newTask;

	testDB.insertTask(newTask);

	auto result = testDB.getTasks();
	auto testTask = result->at(1);

	LONGS_EQUAL(1,testTask->getTaskId());
}

TEST(AppDBGroup, ValidateTaskInsertByName)
{
	Core::Task newTask;

	newTask.setName("test task");
	unsigned int taskId = testDB.insertTask(newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	STRCMP_EQUAL(newTask.getName().c_str(),retrievedTask->getName().c_str());
}

TEST(AppDBGroup, ValidateTaskInsertByEarliestStartTime)
{
    Core::Task newTask;
    
    newTask.setEarliestStartTime(2);
    unsigned int taskId = testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getEarliestStartTime(),retrievedTask->getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskInsertByLatestEndTime)
{
    Core::Task newTask;
    
    newTask.setLatestEndTime(2);
    unsigned int taskId = testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getLatestEndTime(),retrievedTask->getLatestEndTime());
}

TEST(AppDBGroup, ValidateTaskInsertByDuration)
{
    Core::Task newTask;
    
    newTask.setDuration(2);
    unsigned int taskId = testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getDuration(),retrievedTask->getDuration());
}

TEST(AppDBGroup, ValidateTaskInsertByRecurringChildrenCount)
{
	auto newTask = std::make_shared<Core::Task>("",0,50,5);

	newTask->setRecurranceParameters(10,5);
	unsigned int taskId = testDB.insertTask(*newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	LONGS_EQUAL(newTask->getRecurringTaskCount(), retrievedTask->getRecurringTaskCount());
}

TEST(AppDBGroup, ValidateTaskDelete)
{
    Core::Task newTask;
    
    uint64_t taskId = testDB.insertTask(newTask);
    testDB.removeTask(taskId);
    
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(0, result->size());
}

TEST(AppDBGroup, ValidateTaskUpdateByName)
{
	Core::Task newTask;

	auto taskId = testDB.insertTask(newTask);

	newTask.setName("test name");
	testDB.updateTask(taskId, newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	STRCMP_EQUAL(newTask.getName().c_str(),retrievedTask->getName().c_str());
}

TEST(AppDBGroup, ValidateTaskUpdateByEarliestStartTime)
{
    Core::Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setEarliestStartTime(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getEarliestStartTime(),retrievedTask->getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskUpdateByLatestEndTime)
{
    Core::Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setLatestEndTime(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getLatestEndTime(),retrievedTask->getLatestEndTime());
}

TEST(AppDBGroup, ValidateTaskUpdateByDuration)
{
    Core::Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setDuration(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    auto retrievedTask = result->at(taskId);
    LONGS_EQUAL(newTask.getDuration(),retrievedTask->getDuration());
}

TEST(AppDBGroup, ValidateTaskUpdateByRecurringChildrenCount)
{
	auto newTask = std::make_shared<Core::Task>("",0,50,5);

	newTask->setRecurranceParameters(10,5);
	unsigned int taskId = testDB.insertTask(*newTask);

	newTask->setRecurranceParameters(5,0);
	testDB.updateTask(taskId, *newTask);

	auto result = testDB.getTasks();
	auto retrievedTask = result->at(taskId);
	LONGS_EQUAL(newTask->getRecurringTaskCount(), retrievedTask->getRecurringTaskCount());
}

TEST(AppDBGroup, ValidateEventsTableExists)
{
	auto result = testDB.getLoggedEvents();

	LONGS_EQUAL(0, result->size());
}
