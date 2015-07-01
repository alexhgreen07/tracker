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

TEST(AppDBGroup, ValidateTaskInsertByName)
{
	Core::Task newTask;

	newTask.setName("test task");
	testDB.insertTask(newTask);

	auto result = testDB.getTasks();
	STRCMP_EQUAL(newTask.getName().c_str(),result->at(1).getName().c_str());
}

TEST(AppDBGroup, ValidateTaskInsertByEarliestStartTime)
{
    Core::Task newTask;
    
    newTask.setEarliestStartTime(2);
    testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getEarliestStartTime(),result->at(1).getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskInsertByLatestEndTime)
{
    Core::Task newTask;
    
    newTask.setLatestEndTime(2);
    testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getLatestEndTime(),result->at(1).getLatestEndTime());
}

TEST(AppDBGroup, ValidateTaskInsertByDuration)
{
    Core::Task newTask;
    
    newTask.setDuration(2);
    testDB.insertTask(newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getDuration(),result->at(1).getDuration());
}

TEST(AppDBGroup, ValidateTaskDelete)
{
    Core::Task newTask;
    
    uint64_t taskId = testDB.insertTask(newTask);
    testDB.removeTask(taskId);
    
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(0, result->size());
}

TEST(AppDBGroup, ValidateTaskUpdateByEarliestStartTime)
{
    Core::Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setEarliestStartTime(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getEarliestStartTime(),result->at(1).getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskUpdateByLatestEndTime)
{
    Core::Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setLatestEndTime(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getLatestEndTime(),result->at(1).getLatestEndTime());
}

TEST(AppDBGroup, ValidateTaskUpdateByDuration)
{
    Core::Task newTask;
    
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setDuration(2);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getDuration(),result->at(1).getDuration());
}
