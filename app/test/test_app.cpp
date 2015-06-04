#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "app.hpp"

using namespace Tracker::Application;

TEST_GROUP(AppGroup)
{
};

TEST(AppGroup, BasicInitialize)
{
    TrackerApp app;
    CHECK(app.start());
}

