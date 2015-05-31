#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include "app.hpp"

TEST_GROUP(AppGroup)
{
};

TEST(AppGroup, BasicInitialize)
{
    TrackerApp app;
    app.run();
}

