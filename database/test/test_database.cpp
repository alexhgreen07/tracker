#include <CppUTest/TestHarness.h>

#include "database.hpp"

TEST_GROUP(DatabaseGroup)
{
    std::shared_ptr<Database> testDatabase;
    
    void setup()
    {
        testDatabase = std::make_shared<Database>();
    }
};

TEST(DatabaseGroup, BasicInitialize)
{

}
