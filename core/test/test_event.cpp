#include <CppUTest/TestHarness.h>

#include "event.hpp"

using namespace Tracker::Core;

TEST_GROUP(EventGroup)
{
    std::shared_ptr<Event> testEvent;
    
    void setup()
    {
        testEvent = std::make_shared<Event>();
    }
};

TEST(EventGroup, BasicInitialize)
{
    LONGS_EQUAL(0,testEvent->getDuration());
    LONGS_EQUAL(0,testEvent->getStartTime());
    CHECK(!testEvent->getParent());
    CHECK(testEvent->getStatus() == Event::Status::Scheduled);
}

TEST(EventGroup, SetStartTime)
{
    unsigned int testStartTime = 10;
    testEvent->setStartTime(testStartTime);
    
    LONGS_EQUAL(testStartTime,testEvent->getStartTime());
}

TEST(EventGroup, SetDuration)
{
    unsigned int testDuration = 10;
    testEvent->setDuration(testDuration);
    
    LONGS_EQUAL(testDuration,testEvent->getDuration());
}

TEST(EventGroup, SetParentTask)
{
    auto testTask = std::make_shared<Task>();
    testEvent->setParent(testTask);
    
    CHECK(testEvent->getParent().get() == testTask.get());
}

TEST(EventGroup, CheckNotOverlapping)
{
    unsigned int testStartTime = 10;
    unsigned int testDuration = 10;
    unsigned int testOverlapTime = testStartTime + testDuration;
    auto testNonOverlappingEvent = std::make_shared<Event>(testOverlapTime,testDuration);
    
    testEvent->setStartTime(testStartTime);
    testEvent->setDuration(testDuration);
    
    CHECK_FALSE(testEvent->overlaps(testNonOverlappingEvent));
}

TEST(EventGroup, CheckOverlapping)
{
    unsigned int testStartTime = 10;
    unsigned int testDuration = 10;
    unsigned int testOverlapTime = testStartTime + (testDuration / 2);
    auto testOverlappingEvent = std::make_shared<Event>(testOverlapTime,testDuration);
    
    testEvent->setStartTime(testStartTime);
    testEvent->setDuration(testDuration);
    
    CHECK(testEvent->overlaps(testOverlappingEvent));
}

TEST(EventGroup, SetStatus)
{
	Event::Status desiredStatus = Event::Status::Logged;

	testEvent->setStatus(desiredStatus);
	CHECK(testEvent->getStatus() == desiredStatus);
}

