#include <string>
#include <map>
#include <stdlib.h>

#include <database.hpp>
#include <task.hpp>

namespace Tracker
{
namespace Application
{
    
class AppDB
{
public:
    AppDB(Database::Database & database) :
        database(database),
        currentVersion("0")
    {}
    
    void initializeNewDatabase()
    {
        database.execute("create table version (version int)");
        
        createVersionTable();
        createTasksTable();
    }
    
    void createVersionTable()
    {
        database.execute("insert into version values (" + currentVersion + ")");
    }
    
    void createTasksTable()
    {
        std::string createSql = "create table tasks (";
        createSql += "taskId integer primary key asc";
        createSql += ",earliestStartTime real";
        createSql += ",latestEndTime real";
        createSql += ",duration real";
        createSql += ");";
        
        database.execute(createSql);
    }
    
    std::shared_ptr<std::map<uint64_t, Core::Task>> getTasks()
    {
        auto tasksTable = database.select(
            "select taskId, earliestStartTime, latestEndTime, duration from tasks");
        auto tasks = std::make_shared<std::map<uint64_t, Core::Task>>();
        
        for(unsigned int i = 0; i < tasksTable->size(); i++)
        {
            Core::Task nextTask;
            uint64_t taskId = (uint64_t)atoll(tasksTable->at(i)[0].c_str());
            
            nextTask.setEarliestStartTime(atoi(tasksTable->at(i)[1].c_str()));
            nextTask.setLatestEndTime(atoi(tasksTable->at(i)[2].c_str()));
            nextTask.setDuration(atoi(tasksTable->at(i)[3].c_str()));
            
            tasks->insert(std::pair<uint64_t, Core::Task>(taskId,nextTask));
        }
        
        return tasks;
    }
    
    uint64_t insertTask(const Core::Task & newTask)
    {
        std::string columnsString = "";
        std::string valuesString = "";
        
        columnsString += "earliestStartTime";
        valuesString += std::to_string(newTask.getEarliestStartTime());
        columnsString += ",latestEndTime";
        valuesString += "," + std::to_string(newTask.getLatestEndTime());
        columnsString += ",duration";
        valuesString += "," + std::to_string(newTask.getDuration());
        
        std::string insertString =
            "insert into tasks (" +
            columnsString + ") values(" + valuesString + ")";
        
        database.execute(insertString);
        return database.lastInsertRowId();
    }
    
    void updateTask(uint64_t taskId, Core::Task & task)
    {
        std::string updateString = "update tasks set ";
        
        updateString += "earliestStartTime = " + std::to_string(task.getEarliestStartTime());
        
        updateString += " where taskId = " + std::to_string(taskId);
        
        database.execute(updateString);
    }
    
    void removeTask(uint64_t taskId)
    {
        std::string deleteString =
        "delete from tasks where taskId = " + std::to_string(taskId);
        
        database.execute(deleteString);
    }
    
    std::string getCurrentVersion()
    {
        return currentVersion;
    }
private:
    Database::Database & database;
    std::string currentVersion;
};

}
}

#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

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
    
    newTask.setEarliestStartTime(2);
    auto taskId = testDB.insertTask(newTask);
    
    newTask.setEarliestStartTime(3);
    testDB.updateTask(taskId, newTask);
    
    auto result = testDB.getTasks();
    LONGS_EQUAL(newTask.getEarliestStartTime(),result->at(1).getEarliestStartTime());
}
