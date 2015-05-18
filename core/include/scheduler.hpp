#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

#include <vector>
#include <memory>

#include "task.hpp"
#include "event.hpp"

class Scheduler
{
public:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> getTaskList()
    {
        return taskList;
    }
    void setTaskList(std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList)
    {
        this->taskList = taskList;
    }
    size_t getScheduledEventCount()
    {
        return scheduledEvents.size();
    }
    void schedule()
    {
        if(taskList)
        {
            std::sort(taskList->begin(),taskList->end(),compareTasks);
            
            scheduledEvents.clear();
            for(unsigned int i = 0; i < taskList->size(); i++)
            {
                auto currentTask = taskList->at(i);
                auto newEvent = std::make_shared<Event>();
                
                newEvent->setParent(currentTask);
                newEvent->setStartTime(currentTask->getEarliestStartTime());
                newEvent->setDuration(currentTask->getDuration());
                
                scheduledEvents.push_back(newEvent);
            }
            
            std::sort(scheduledEvents.begin(),scheduledEvents.end(),compareEvents);
        }
    }
    std::shared_ptr<Event> getScheduledEvent(unsigned int index)
    {
        return scheduledEvents[index];
    }
private:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    std::vector<std::shared_ptr<Event>> scheduledEvents;
    
    static bool compareTasks(std::shared_ptr<Task> a, std::shared_ptr<Task> b)
    {
        return (a->getLatestEndTime() < b->getLatestEndTime());
    }
    static bool compareEvents(std::shared_ptr<Event> a, std::shared_ptr<Event> b)
    {
        return (a->getStartTime() < b->getStartTime());
    }
};

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
    auto newTaskList = std::make_shared<std::vector<std::shared_ptr<Task>>>();
    auto newTask = std::make_shared<Task>();
    const unsigned int startTime = 10;
    
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

#endif