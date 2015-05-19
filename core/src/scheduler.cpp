#include "scheduler.hpp"

std::shared_ptr<std::vector<std::shared_ptr<Task>>> Scheduler::getTaskList()
{
    return taskList;
}
void Scheduler::setTaskList(std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList)
{
    this->taskList = taskList;
}
size_t Scheduler::getScheduledEventCount()
{
    return scheduledEvents.size();
}
void Scheduler::schedule()
{
    if(taskList)
    {
        unsigned int earliestFreeTime = 0;
        
        std::sort(taskList->begin(),taskList->end(),compareTasks);
        scheduledEvents.clear();
        
        for(unsigned int i = 0; i < taskList->size(); i++)
        {
            auto currentTask = taskList->at(i);
            auto newEvents = scheduleInFreeSpace(currentTask,earliestFreeTime);
            
            for(unsigned int j = 0; j < newEvents->size(); j++)
            {
                auto newEvent = newEvents->at(j);
                auto upperBound =
                    std::upper_bound(scheduledEvents.begin(),scheduledEvents.end(),newEvent,compareEvents);
                scheduledEvents.insert(upperBound,newEvent);
            }
        }
    }
}
std::shared_ptr<Event> Scheduler::getScheduledEvent(unsigned int index)
{
    return scheduledEvents[index];
}

std::shared_ptr<std::vector<std::shared_ptr<Event>>>
    Scheduler::scheduleInFreeSpace(std::shared_ptr<Task> currentTask, unsigned int & earliestFreeTime)
{
    auto scheduledEvents = std::make_shared<std::vector<std::shared_ptr<Event>>>();
    
    auto newEvent = std::make_shared<Event>();
    unsigned int eventStartTime = 0;
    
    newEvent->setParent(currentTask);
    newEvent->setDuration(currentTask->getDuration());
    
    if(earliestFreeTime <= currentTask->getEarliestStartTime())
    {
        eventStartTime = currentTask->getEarliestStartTime();
    }
    else
    {
        eventStartTime = earliestFreeTime;
    }
    
    newEvent->setStartTime(eventStartTime);
    earliestFreeTime = eventStartTime + currentTask->getDuration();
    
    scheduledEvents->push_back(newEvent);
    
    return scheduledEvents;
}

bool Scheduler::compareTasks(std::shared_ptr<Task> a, std::shared_ptr<Task> b)
{
    return (a->getLatestStartTime() < b->getLatestStartTime());
}
bool Scheduler::compareEvents(std::shared_ptr<Event> a, std::shared_ptr<Event> b)
{
    return (a->getStartTime() < b->getStartTime());
}
