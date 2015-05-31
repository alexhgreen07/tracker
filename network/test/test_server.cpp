#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "server.hpp"

using namespace Tracker::Network;

TEST_GROUP(NetworkServerGroup)
{
};

TEST(NetworkServerGroup, BasicStart)
{
    Server server;
    CHECK(server.start());
}