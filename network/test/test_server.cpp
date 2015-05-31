#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "server.hpp"

TEST_GROUP(NetworkServerGroup)
{
};

TEST(NetworkServerGroup, BasicStart)
{
    Server server;
    CHECK(server.start());
}