#include <CppUTest/TestHarness.h>

#include "database.hpp"

TEST_GROUP(DatabaseGroup)
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

TEST(DatabaseGroup, BasicOpen)
{
    testDatabase->open("./test.sqlite3");
    CHECK(testDatabase->isConnected());
}

TEST(DatabaseGroup, BasicClose)
{
    testDatabase->open("./test.sqlite3");
    testDatabase->close();
    CHECK_FALSE(testDatabase->isConnected());
}
