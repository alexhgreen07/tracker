#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "server.hpp"

using namespace Tracker::Network;

struct DummyApi : public Api
{
	JsonMethods & getProcedures()
	{
		return procedurePointers;
	}
	JsonNotifications & getNotifications()
	{
		return notPointers;
	}
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

TEST_GROUP(NetworkServerGroup)
{
};

TEST(NetworkServerGroup, BasicStart)
{
	DummyApi api;
    Server server(api);
    CHECK(server.start());
}