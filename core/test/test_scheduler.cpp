#include <CppUTest/TestHarness.h>

#include "scheduler.hpp"

TEST_GROUP(SchedulerGroup)
{
    std::shared_ptr<Scheduler> testScheduler;
    
    void setup()
    {
        testScheduler = std::make_shared<Scheduler>();
    }
};

TEST(SchedulerGroup, BasicInitialize)
{
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    
    taskList = testScheduler->getTaskList();
    CHECK(!taskList);
    LONGS_EQUAL(0, testScheduler->getScheduledEventCount());
}

TEST(SchedulerGroup, SetTaskList)
{
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    testScheduler->setTaskList(newTaskList);
    
    taskList = testScheduler->getTaskList();
    CHECK(taskList.get() == newTaskList.get());
}

TEST(SchedulerGroup, ScheduleSingleEvent)
{
    const unsigned int startTime = 10;
    
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto newTask = std::make_shared<Task>();
    newTask->setEarliestStartTime(startTime);
    newTask->setLatestEndTime(startTime);
    newTask->setDuration(15);
    newTaskList->push_back(newTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK(newTask.get() == event->getParent().get());
    LONGS_EQUAL(newTask->getEarliestStartTime(), event->getStartTime());
    LONGS_EQUAL(newTask->getDuration(), event->getDuration());
}

TEST(SchedulerGroup, ScheduleMultipleNonOverlappingEvents)
{
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    const unsigned int duration = 10;
    
    for(unsigned int i = 0; i < 5; i++)
    {
        auto newTask = std::make_shared<Task>();
        newTask->setEarliestStartTime(i * duration);
        newTask->setLatestEndTime(i * duration);
        newTask->setDuration(duration);
        newTaskList->push_back(newTask);
    }
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    for(unsigned int i = 0; i < testScheduler->getScheduledEventCount(); i++)
    {
        auto event = testScheduler->getScheduledEvent(i);
        CHECK(newTaskList->at(i).get() == event->getParent().get());
        LONGS_EQUAL(newTaskList->at(i)->getEarliestStartTime(), event->getStartTime());
        LONGS_EQUAL(newTaskList->at(i)->getDuration(), event->getDuration());
    }
}

TEST(SchedulerGroup, ScheduleBasicOverlappingEvents)
{
    const unsigned int firstEarlyStartTime = 10;
    const unsigned int firstLatestStartTime = 20;
    const unsigned int lastEarlyStartTime = 20;
    const unsigned int lastLatestStartTime = 30;
    const unsigned int duration = 15;
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto firstTask = std::make_shared<Task>();
    firstTask->setEarliestStartTime(firstEarlyStartTime);
    firstTask->setLatestEndTime(firstLatestStartTime);
    firstTask->setDuration(duration);
    newTaskList->push_back(firstTask);
    
    auto lastTask = std::make_shared<Task>();
    lastTask->setEarliestStartTime(lastEarlyStartTime);
    lastTask->setLatestEndTime(lastLatestStartTime);
    lastTask->setDuration(duration);
    newTaskList->push_back(lastTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();

    auto event = testScheduler->getScheduledEvent(0);
    CHECK(firstTask.get() == event->getParent().get());
    LONGS_EQUAL(firstEarlyStartTime, event->getStartTime());
    event = testScheduler->getScheduledEvent(1);
    CHECK(lastTask.get() == event->getParent().get());
    LONGS_EQUAL(firstEarlyStartTime + duration, event->getStartTime());
}
