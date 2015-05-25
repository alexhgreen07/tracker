#include <CppUTest/TestHarness.h>

#include <cstdio>

#include "database.hpp"

static std::string dbName = ":memory:";
static std::string createTableTestSql = "CREATE TABLE persons(personID int);";
static std::string dropTableTestSql = "DROP TABLE persons;";

TEST_GROUP(Sqlite3Group)
{
    std::shared_ptr<DatabaseSqlite3> testDatabase;
    std::string dbName;
    
    void setup()
    {
        testDatabase = std::make_shared<DatabaseSqlite3>();
        testDatabase->open(dbName);
    }
    
    void teardown()
    {
        testDatabase->close();
    }
    
    void createTestTable()
    {
        testDatabase->execute(createTableTestSql);
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
        testDatabase->execute(createTableTestSql);
    } catch (std::exception & exc) {
        FAIL("Create table threw exception");
    }
}

TEST(Sqlite3Group, DropTable)
{
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
        LONGS_EQUAL(1, row->size());
        
        auto resultVersion = data->at(0)->at(0);
        CHECK(resultVersion == "3.8.11");
        
    } catch (std::exception & exc) {
        FAIL("Failed to execute select");
    }
}
