#include <string>

#include <database.hpp>

namespace Tracker
{
namespace Application
{

using namespace Database;
    
class AppDB : public DatabaseSqlite3
{
public:
    AppDB() :
        currentVersion("0")
    {}
    
    int initializeNewDatabase()
    {
        int returnValue = 0;
        
        execute("create table version (version varchar(10))");
        execute("insert into version values (\"" + currentVersion + "\")");
        
        return returnValue;
    }
    
    std::string getCurrentVersion()
    {
        return currentVersion;
    }
private:
    std::string currentVersion;
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

TEST(AppDBGroup, InitializeNewDatabase)
{
    testDB.initializeNewDatabase();
    
    auto result = testDB.select("select version from version");
    
    STRCMP_EQUAL(result->at(0)[0].c_str(),testDB.getCurrentVersion().c_str());
}
