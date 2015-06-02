#include <string>

#include <database.hpp>

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
        database.execute("insert into version values (" + currentVersion + ")");
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
    }
    
    TEST_TEARDOWN()
    {
        mysqlDB.close();
    }
};

TEST(AppDBGroup, InitializeNewDatabase)
{
    testDB.initializeNewDatabase();
    
    auto result = mysqlDB.select("select version from version");
    
    STRCMP_EQUAL(result->at(0)[0].c_str(),testDB.getCurrentVersion().c_str());
}
