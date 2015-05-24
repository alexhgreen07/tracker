#include <CppUTest/TestHarness.h>

#include <cstdio>

#include "database.hpp"

TEST_GROUP(Sqlite3Group)
{
    std::shared_ptr<DatabaseSqlite3> testDatabase;
    std::string dbName;
    
    void setup()
    {
        dbName = "./test.sqlite3";
        
        testDatabase = std::make_shared<DatabaseSqlite3>();
        testDatabase->open(dbName);
    }
    
    void teardown()
    {
        testDatabase->close();
        std::remove(dbName.c_str());
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
        testDatabase->execute("CREATE TABLE Persons(PersonID int);");
    } catch (std::exception & exc) {
        FAIL("Create table threw exception");
    }
}
