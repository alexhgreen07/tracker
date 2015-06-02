#include <database.hpp>

namespace Tracker
{
namespace Application
{

using namespace Database;
    
class AppDB : public DatabaseSqlite3
{
public:
    AppDB()
    {}
};

}
}

#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

using namespace Tracker::Application;

TEST_GROUP(AppDBGroup)
{
    AppDB testDB;
    
    TEST_SETUP()
    {
        testDB.open(":memory:");
    }
    
    TEST_TEARDOWN()
    {
        testDB.close();
    }
};

TEST(AppDBGroup, BasicOpen)
{
    CHECK(testDB.isConnected());
}
