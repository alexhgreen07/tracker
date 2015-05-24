#include <CppUTest/TestHarness.h>

#include "database.hpp"

TEST_GROUP(Sqlite3Group)
{
    std::shared_ptr<DatabaseSqlite3> testDatabase;
    
    void setup()
    {
        testDatabase = std::make_shared<DatabaseSqlite3>();
    }
    
    void teardown()
    {
        testDatabase->close();
    }
};

TEST(Sqlite3Group, BasicOpen)
{
    testDatabase->open("./test.sqlite3");
    CHECK(testDatabase->isConnected());
}

TEST(Sqlite3Group, BasicClose)
{
    testDatabase->open("./test.sqlite3");
    testDatabase->close();
    CHECK_FALSE(testDatabase->isConnected());
}
