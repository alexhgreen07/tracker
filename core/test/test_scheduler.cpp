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
    const unsigned int duration = 10;
    const unsigned int startTime = 10;
    
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto newTask = std::make_shared<Task>(startTime,startTime + duration,duration);
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
        auto newTask =
            std::make_shared<Task>(i * duration,i * duration + duration,duration);
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
    const unsigned int duration = 15;
    const unsigned int firstEarlyStartTime = 10;
    const unsigned int firstLatestEndTime = (firstEarlyStartTime + duration);
    const unsigned int lastEarlyStartTime = 20;
    const unsigned int lastLatestEndTime = (lastEarlyStartTime + duration) + duration;
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto firstTask =
        std::make_shared<Task>(firstEarlyStartTime,firstLatestEndTime,duration);
    newTaskList->push_back(firstTask);
    
    auto lastTask =
        std::make_shared<Task>(lastEarlyStartTime,lastLatestEndTime,duration);
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

TEST(SchedulerGroup, ScheduleOverlappingEventsWithSplit)
{
    const unsigned int firstEarlyStartTime = 20;
    const unsigned int firstDuration = 15;
    const unsigned int firstLatestEndTime = firstEarlyStartTime + firstDuration;
    const unsigned int lastEarlyStartTime = 0;
    const unsigned int lastDuration = 30;
    const unsigned int lastLatestEndTime = (lastEarlyStartTime + lastDuration) + firstDuration;
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto firstTask =
        std::make_shared<Task>(firstEarlyStartTime,firstLatestEndTime,firstDuration);
    newTaskList->push_back(firstTask);
    
    auto lastTask =
        std::make_shared<Task>(lastEarlyStartTime,lastLatestEndTime,lastDuration);
    newTaskList->push_back(lastTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK(lastTask.get() == event->getParent().get());
    LONGS_EQUAL(lastEarlyStartTime, event->getStartTime());
    event = testScheduler->getScheduledEvent(1);
    CHECK(firstTask.get() == event->getParent().get());
    LONGS_EQUAL(firstEarlyStartTime, event->getStartTime());
    event = testScheduler->getScheduledEvent(2);
    CHECK(lastTask.get() == event->getParent().get());
    LONGS_EQUAL(firstLatestEndTime, event->getStartTime());
}

TEST(SchedulerGroup, ScheduleThreeOverlappingEventsWithSplit)
{
    const unsigned int firstEarlyStartTime = 0;
    const unsigned int firstDuration = 30;
    const unsigned int firstLatestEndTime = firstEarlyStartTime + firstDuration + 60;
    const unsigned int secondEarlyStartTime = 20;
    const unsigned int secondDuration = 15;
    const unsigned int secondLatestEndTime = secondEarlyStartTime + secondDuration;
    const unsigned int lastEarlyStartTime = 10;
    const unsigned int lastDuration = 30;
    const unsigned int lastLatestEndTime = lastEarlyStartTime + lastDuration + 20;
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto firstTask =
    std::make_shared<Task>(firstEarlyStartTime,firstLatestEndTime,firstDuration);
    newTaskList->push_back(firstTask);
    
    auto secondTask =
    std::make_shared<Task>(secondEarlyStartTime,secondLatestEndTime,secondDuration);
    newTaskList->push_back(secondTask);
    
    auto lastTask =
    std::make_shared<Task>(lastEarlyStartTime,lastLatestEndTime,lastDuration);
    newTaskList->push_back(lastTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK(firstTask.get() == event->getParent().get());
    LONGS_EQUAL(firstEarlyStartTime, event->getStartTime());
    
    event = testScheduler->getScheduledEvent(1);
    CHECK(lastTask.get() == event->getParent().get());
    LONGS_EQUAL(lastEarlyStartTime, event->getStartTime());

    event = testScheduler->getScheduledEvent(2);
    CHECK(secondTask.get() == event->getParent().get());
    LONGS_EQUAL(secondEarlyStartTime, event->getStartTime());
    
    event = testScheduler->getScheduledEvent(3);
    CHECK(lastTask.get() == event->getParent().get());
    LONGS_EQUAL(secondLatestEndTime, event->getStartTime());
    
    event = testScheduler->getScheduledEvent(4);
    CHECK(firstTask.get() == event->getParent().get());
    
    for(unsigned int i = 1; i < testScheduler->getScheduledEventCount(); i++)
    {
        auto lastEvent = testScheduler->getScheduledEvent(i - 1);
        auto event = testScheduler->getScheduledEvent(i);
        
        CHECK_FALSE(event->overlaps(lastEvent));
    }
}
