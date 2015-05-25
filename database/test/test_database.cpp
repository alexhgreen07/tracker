#include <CppUTest/TestHarness.h>

#include <cstdio>

#include "database.hpp"

static std::string dbName = "./test.sqlite3";
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
        std::remove(dbName.c_str());
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
