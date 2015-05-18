#include <CppUTest/TestHarness.h>

#include "event.hpp"

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
    LONGS_EQUAL(0,testEvent->getEarliestStartTime());
    LONGS_EQUAL(0,testEvent->getLatestEndTime());
    LONGS_EQUAL(0,testEvent->getDuration());
    LONGS_EQUAL(0,testEvent->getChildrenCount());
    CHECK(testEvent->getParent().expired());
}

TEST(EventGroup, SetEarliestStartTime)
{
    unsigned int testTime = 10;
    testEvent->setEarliestStartTime(testTime);
    
    LONGS_EQUAL(testTime,testEvent->getEarliestStartTime());
}

TEST(EventGroup, SetLatestEndTime)
{
    unsigned int testTime = 15;
    testEvent->setLatestEndTime(testTime);
    
    LONGS_EQUAL(testTime,testEvent->getLatestEndTime());
}

TEST(EventGroup, SetDuration)
{
    unsigned int testDuration = 20;
    testEvent->setDuration(testDuration);
    
    LONGS_EQUAL(testDuration, testEvent->getDuration())
}

TEST(EventGroup, AddChild)
{
    std::shared_ptr<Event> newChild = std::make_shared<Event>();
    testEvent->addChild(newChild);
    
    LONGS_EQUAL(1,testEvent->getChildrenCount());
}

TEST(EventGroup, GetChild)
{
    std::shared_ptr<Event> newChild = std::make_shared<Event>();
    testEvent->addChild(newChild);
    std::shared_ptr<Event> child = testEvent->getChild(0);
    
    CHECK(child.get() == newChild.get());
}

TEST(EventGroup, RemoveChild)
{
    std::shared_ptr<Event> newChild = std::make_shared<Event>();
    testEvent->addChild(newChild);
    testEvent->removeChild(0);
    
    LONGS_EQUAL(0,testEvent->getChildrenCount());
}

TEST(EventGroup, GetParentofChild)
{
    std::shared_ptr<Event> newChild = std::make_shared<Event>();
    testEvent->addChild(newChild);
    std::shared_ptr<Event> childParent = std::shared_ptr<Event>(newChild->getParent());
    
    CHECK(childParent.get() == testEvent.get());
}

TEST(EventGroup, CheckParentofChildAfterRemoval)
{
    std::shared_ptr<Event> newChild = std::make_shared<Event>();
    testEvent->addChild(newChild);
    testEvent->removeChild(0);
    std::weak_ptr<Event> childParent = newChild->getParent();
    
    CHECK(childParent.expired());
}
