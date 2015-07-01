#include <CppUTest/TestHarness.h>

#include "scheduler.hpp"

using namespace Tracker::Core;

#define CHECK_EVENT(expectedParent,expectedStartTime,expectedDuration,event) \
    CHECK(expectedParent == event->getParent()); \
    LONGS_EQUAL(expectedStartTime, event->getStartTime()); \
    LONGS_EQUAL(expectedDuration, event->getDuration());

#define CHECK_NO_OVERLAP() \
    CHECK_FALSE(eventsOverlap());

TEST_GROUP(SchedulerGroup)
{
    std::shared_ptr<Scheduler> testScheduler;
    
    void setup()
    {
        testScheduler = std::make_shared<Scheduler>();
    }
    
    std::shared_ptr<std::vector<std::shared_ptr<Task>>>
        createNonOverlappingTasks(unsigned int duration, unsigned int count, unsigned int space = 0)
    {
        auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
        unsigned int spacedDuration = (duration + space);
        
        for(unsigned int i = 0; i < count; i++)
        {
            auto newTask =
                std::make_shared<Task>("",i * spacedDuration,i * spacedDuration + duration,duration);
            newTaskList->push_back(newTask);
        }
        
        return newTaskList;
    }
    
    void scheduleNonOverlappingTasks(unsigned int duration, unsigned int count, unsigned int space = 0)
    {
        auto newTaskList = createNonOverlappingTasks(duration,count,space);
        
        testScheduler->setTaskList(newTaskList);
        testScheduler->schedule();
    }
    
    void scheduleBasicOverlappingTasks(unsigned int firstEarlyStartTime, unsigned int duration)
    {
        const unsigned int firstLatestEndTime = (firstEarlyStartTime + duration);
        const unsigned int lastEarlyStartTime = firstLatestEndTime - (duration / 2);
        const unsigned int lastLatestEndTime = (lastEarlyStartTime + duration) + duration;
        auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
        
        auto firstTask =
        std::make_shared<Task>("",firstEarlyStartTime,firstLatestEndTime,duration);
        newTaskList->push_back(firstTask);
        
        auto lastTask =
        std::make_shared<Task>("",lastEarlyStartTime,lastLatestEndTime,duration);
        newTaskList->push_back(lastTask);
        
        testScheduler->setTaskList(newTaskList);
        testScheduler->schedule();
    }
    
    void scheduleOverlappingSplitTasks(unsigned int firstDuration, unsigned int lastEarlyStartTime)
    {
        const unsigned int firstEarlyStartTime = firstDuration;
        const unsigned int firstLatestEndTime = firstEarlyStartTime + firstDuration;
        const unsigned int lastDuration = firstDuration * 2;
        const unsigned int lastLatestEndTime = (lastEarlyStartTime + lastDuration) + firstDuration;
        auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
        
        auto firstTask =
        std::make_shared<Task>("",firstEarlyStartTime,firstLatestEndTime,firstDuration);
        newTaskList->push_back(firstTask);
        
        auto lastTask =
        std::make_shared<Task>("",lastEarlyStartTime,lastLatestEndTime,lastDuration);
        newTaskList->push_back(lastTask);
        
        testScheduler->setTaskList(newTaskList);
        testScheduler->schedule();
    }
    
    bool eventsOverlap()
    {
        for(unsigned int i = 1; i < testScheduler->getScheduledEventCount(); i++)
        {
            auto lastEvent = testScheduler->getScheduledEvent(i - 1);
            auto event = testScheduler->getScheduledEvent(i);
            
            if(event->overlaps(lastEvent))
                return true;
        }
        
        return false;
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
    
    CHECK(taskList == newTaskList);
}

TEST(SchedulerGroup, ScheduleSingleEvent)
{
    const unsigned int duration = 10;
    const unsigned int count = 1;
    scheduleNonOverlappingTasks(duration,count);
    
    LONGS_EQUAL(count,testScheduler->getScheduledEventCount());
    
    auto newTask = testScheduler->getTaskList()->at(0);
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK_EVENT(newTask,newTask->getEarliestStartTime(),newTask->getDuration(),event);
}

TEST(SchedulerGroup, ScheduleMultipleNonOverlappingEvents)
{
    const unsigned int duration = 10;
    const unsigned int count = 5;
    scheduleNonOverlappingTasks(duration,count);
    
    LONGS_EQUAL(count,testScheduler->getScheduledEventCount());
    
    for(unsigned int i = 0; i < count; i++)
    {
        auto taskToCheck = testScheduler->getTaskList()->at(i);
        
        auto event = testScheduler->getScheduledEvent(i);
        CHECK_EVENT(taskToCheck,taskToCheck->getEarliestStartTime(),taskToCheck->getDuration(),event);
    }
}

TEST(SchedulerGroup, ScheduleBasicOverlappingEvents)
{
    const unsigned int duration = 15;
    const unsigned int earliestStartTime = 0;
    scheduleBasicOverlappingTasks(earliestStartTime,duration);

    auto lastTask = testScheduler->getTaskList()->at(1);
    
    auto event = testScheduler->getScheduledEvent(1);
    CHECK_EVENT(lastTask,earliestStartTime + duration,duration,event);
    
    CHECK_NO_OVERLAP();
}

TEST(SchedulerGroup, ScheduleOverlappingEventsWithSplit)
{
    const unsigned int firstDuration = 15;
    const unsigned int lastEarlyStartTime = 0;
    scheduleOverlappingSplitTasks(firstDuration,lastEarlyStartTime);
    
    auto firstTask = testScheduler->getTaskList()->at(0);
    auto lastTask = testScheduler->getTaskList()->at(1);
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK_EVENT(lastTask,lastTask->getEarliestStartTime(),firstTask->getEarliestStartTime(),event);
    event = testScheduler->getScheduledEvent(1);
    CHECK_EVENT(firstTask,firstTask->getEarliestStartTime(),firstDuration,event);
    event = testScheduler->getScheduledEvent(2);
    CHECK_EVENT(lastTask,firstTask->getLatestEndTime(),lastTask->getDuration() - firstDuration,event);
    
    CHECK_NO_OVERLAP();
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
        std::make_shared<Task>("",firstEarlyStartTime,firstLatestEndTime,firstDuration);
    newTaskList->push_back(firstTask);
    
    auto secondTask =
        std::make_shared<Task>("",secondEarlyStartTime,secondLatestEndTime,secondDuration);
    newTaskList->push_back(secondTask);
    
    auto lastTask =
        std::make_shared<Task>("",lastEarlyStartTime,lastLatestEndTime,lastDuration);
    newTaskList->push_back(lastTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK_EVENT(firstTask,firstEarlyStartTime,lastEarlyStartTime,event);
    
    event = testScheduler->getScheduledEvent(1);
    CHECK_EVENT(lastTask,lastEarlyStartTime,secondEarlyStartTime - lastEarlyStartTime,event);
    
    event = testScheduler->getScheduledEvent(2);
    CHECK_EVENT(secondTask,secondEarlyStartTime,secondLatestEndTime - secondEarlyStartTime,event);
    
    event = testScheduler->getScheduledEvent(3);
    CHECK(lastTask == event->getParent());
    LONGS_EQUAL(secondLatestEndTime, event->getStartTime());
    
    event = testScheduler->getScheduledEvent(4);
    CHECK(firstTask == event->getParent());
    
    CHECK_NO_OVERLAP();
}

TEST(SchedulerGroup, ScheduleConflictingTasks)
{
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto firstTask =
        std::make_shared<Task>("",0,10,10);
    newTaskList->push_back(firstTask);
    
    auto lastTask =
        std::make_shared<Task>("",5,15,10);
    newTaskList->push_back(lastTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    LONGS_EQUAL(3,testScheduler->getScheduledEventCount());
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK_EVENT(firstTask,0,10,event);
    event = testScheduler->getScheduledEvent(1);
    CHECK_EVENT(lastTask,10,5,event);
    event = testScheduler->getScheduledEvent(2);
    CHECK_EVENT(lastTask,15,5,event);
    
    CHECK_NO_OVERLAP();
}

TEST(SchedulerGroup, ScheduleThreeConflictingTasks)
{
    const unsigned int duration = 10;
    
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    auto firstTask =
    std::make_shared<Task>("",0,10,duration);
    newTaskList->push_back(firstTask);
    
    auto secondTask =
    std::make_shared<Task>("",5,15,duration);
    newTaskList->push_back(secondTask);
    
    auto lastTask =
    std::make_shared<Task>("",10,20,duration);
    newTaskList->push_back(lastTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    LONGS_EQUAL(4,testScheduler->getScheduledEventCount());
    
    auto event = testScheduler->getScheduledEvent(0);
    CHECK_EVENT(firstTask,0,10,event);
    event = testScheduler->getScheduledEvent(1);
    CHECK_EVENT(secondTask,10,5,event);
    event = testScheduler->getScheduledEvent(2);
    CHECK_EVENT(secondTask,15,5,event);
    event = testScheduler->getScheduledEvent(3);
    CHECK_EVENT(lastTask,20,10,event);
    
    CHECK_NO_OVERLAP();
}

TEST(SchedulerGroup, ScheduleBasicRecurringTask)
{
    const unsigned int count = 4;
    const unsigned int duration = 5;
    const unsigned int period = 10;
    const unsigned int latestEndTime = (count) * period;
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    auto newTask = std::make_shared<Task>("",0,latestEndTime,duration);
    
    newTask->setRecurranceParameters(period, duration);
    newTaskList->push_back(newTask);
    
    testScheduler->setTaskList(newTaskList);
    testScheduler->schedule();
    
    LONGS_EQUAL(count,testScheduler->getScheduledEventCount());
    
    for(unsigned int i = 0; i < count; i++)
    {
        auto taskToCheck = newTask->getRecurringChild(i);
        
        auto event = testScheduler->getScheduledEvent(i);
        CHECK_EVENT(taskToCheck,taskToCheck->getEarliestStartTime(),taskToCheck->getDuration(),event);
    }
}
