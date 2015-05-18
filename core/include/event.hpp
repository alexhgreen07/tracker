#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <memory>

#include "task.hpp"

class Event
{
public:
    Event() :
        startTime(0),
        duration(0)
    {}
    unsigned int getDuration()
    {
        return duration;
    }
    void setDuration(unsigned int duration)
    {
        this->duration = duration;
    }
    unsigned int getStartTime()
    {
        return startTime;
    }
    void setStartTime(unsigned int startTime)
    {
        this->startTime = startTime;
    }
    std::shared_ptr<Task> getParent()
    {
        return parent;
    }
    void setParent(std::shared_ptr<Task> parent)
    {
        this->parent = parent;
    }
private:
    unsigned int startTime;
    unsigned int duration;
    std::shared_ptr<Task> parent;
};

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
    std::shared_ptr<Task> testTask = std::make_shared<Task>();
    testEvent->setParent(testTask);
    
    CHECK(testEvent->getParent().get() == testTask.get());
}

#endif