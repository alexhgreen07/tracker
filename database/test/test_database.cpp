#include <CppUTest/TestHarness.h>

#include <cstdio>

#include "database.hpp"

using namespace Tracker::Database;

TEST_GROUP(Sqlite3Group)
{
    std::shared_ptr<DatabaseSqlite3> testDatabase;
    std::string dbName;
    
    void setup()
    {
        std::string dbName = ":memory:";
        testDatabase = std::make_shared<DatabaseSqlite3>();
        testDatabase->open(dbName);
    }
    
    void teardown()
    {
        testDatabase->close();
    }
    
    void createTestTable()
    {
        std::string createTableTestSql = "CREATE TABLE persons(personID int);";
        testDatabase->execute(createTableTestSql);
    }
    
    void insertTestRow()
    {
        std::string insertTestSql = "insert into `persons` values(1)";
        testDatabase->execute(insertTestSql);
    }
};

TEST(Sqlite3Group, BasicOpen)
{
    CHECK(testDatabase->isConnected());
}

TEST(Sqlite3Group, BasicClose)
{
    testDatabase->close();
    CHECK_FALSE(testDatabase->isConnected());
}

TEST(Sqlite3Group, CreateTable)
{
    try {
        createTestTable();
    } catch (std::exception & exc) {
        FAIL("Create table threw exception");
    }
}

TEST(Sqlite3Group, DropTable)
{
    std::string dropTableTestSql = "DROP TABLE persons;";
    
    createTestTable();
    
    try {
        testDatabase->execute(dropTableTestSql);
    } catch (std::exception & exc) {
        FAIL("Create table threw exception");
    }
}

TEST(Sqlite3Group, BasicSelect)
{
    std::string selectTestSql = "select SQLITE_VERSION();";
    
    try {
        auto data = testDatabase->select(selectTestSql);
        
        LONGS_EQUAL(1, data->size());
        
        auto row = data->at(0);
        LONGS_EQUAL(1, row.size());
        
        auto resultVersion = row.at(0);
        CHECK(resultVersion == "3.8.11");
        
    } catch (std::exception & exc) {
        FAIL("Failed to execute select");
    }
}

TEST(Sqlite3Group, BasicInsert)
{
    createTestTable();
    
    try {
        insertTestRow();
        
        auto data = testDatabase->select("select * from `persons`");
        LONGS_EQUAL(1, data->size());
        
        auto row = data->at(0);
        LONGS_EQUAL(1, row.size());
        
        auto resultVersion = row.at(0);
        CHECK(resultVersion == "1");
    } catch (std::exception & exc) {
        FAIL("Insert statement threw exception");
    }
}

TEST(Sqlite3Group, BasicDelete)
{
    std::string deleteTestSql = "delete from `persons`";
    
    createTestTable();
    insertTestRow();
    
    try {
        testDatabase->execute(deleteTestSql);
        
        auto data = testDatabase->select("select * from `persons`");
        LONGS_EQUAL(0, data->size());
        
    } catch (std::exception & exc) {
        FAIL("Insert statement threw exception");
    }
}

TEST(Sqlite3Group, BasicUpdate)
{
    std::string updateTestSql = "update `persons` set personID=2";
    
    createTestTable();
    insertTestRow();
    
    try {
        testDatabase->execute(updateTestSql);
        
        auto data = testDatabase->select("select * from `persons`");
        LONGS_EQUAL(1, data->size());
        
        auto row = data->at(0);
        LONGS_EQUAL(1, row.size());
        
        auto resultVersion = row.at(0);
        CHECK(resultVersion == "2");
    } catch (std::exception & exc) {
        FAIL("Insert statement threw exception");
    }
}
