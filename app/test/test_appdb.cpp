#include <string>
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
    
    std::shared_ptr<std::vector<Core::Task>> getTasks()
    {
        auto tasksTable = database.select("select earliestStartTime from tasks");
        auto tasks = std::make_shared<std::vector<Core::Task>>();
        
        for(unsigned int i = 0; i < tasksTable->size(); i++)
        {
            Core::Task nextTask;
            
            nextTask.setEarliestStartTime(atoi(tasksTable->at(i)[0].c_str()));
            
            tasks->push_back(nextTask);
        }
        
        return tasks;
    }
    
    void insertTask(Core::Task & newTask)
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
    }
    
    void removeTask(unsigned int taskId)
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
    LONGS_EQUAL(newTask.getEarliestStartTime(),result->at(0).getEarliestStartTime());
}

TEST(AppDBGroup, ValidateTaskDelete)
{
    Core::Task newTask;
    
    testDB.insertTask(newTask);
    testDB.removeTask(1);
    
    auto result = testDB.getTasks();
    
    LONGS_EQUAL(0, result->size());
}
