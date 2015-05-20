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
    
    unsigned int remainingDuration = currentTask->getDuration();
    unsigned int nextStartTime = currentTask->getEarliestStartTime();
    
    while(remainingDuration > 0)
    {
        auto newEvent = std::make_shared<Event>();
        unsigned int freeStartTime = 0;
        unsigned int freeDuration = 0;
        
        newEvent->setParent(currentTask);
        
        findFreeSpaceBetween(
                             nextStartTime,
                             currentTask->getLatestEndTime(),
                             freeStartTime,freeDuration);
        
        if(freeDuration > remainingDuration)
        {
            newEvent->setDuration(remainingDuration);
        }
        else
        {
            newEvent->setDuration(freeDuration);
        }
        
        newEvent->setStartTime(freeStartTime);
        
        scheduledEvents->push_back(newEvent);
        
        remainingDuration -= newEvent->getDuration();
        nextStartTime += freeDuration;
    }
    
    return scheduledEvents;
}

bool Scheduler::findFreeSpaceBetween(unsigned int startTime, unsigned int endTime, unsigned int & freeStartTime, unsigned int & freeDuration)
{
    bool found = false;
    auto newEvent = std::make_shared<Event>(startTime,(endTime - startTime));
    
    if(scheduledEvents.size() == 0)
    {
        freeDuration = endTime - startTime;
        freeStartTime = startTime;
        found = true;
    }
    else
    {
        auto upperBound =
            std::lower_bound(scheduledEvents.begin(),scheduledEvents.end(),newEvent,compareEvents);
        
        if(upperBound == scheduledEvents.end())
        {
            upperBound--;
            auto closestEvent = *upperBound;
            
            freeDuration = endTime - closestEvent->getEndTime();
            freeStartTime = closestEvent->getEndTime();
            found = true;
        }
        else
        {
            auto closestEvent = *upperBound;
            
            if(startTime >= closestEvent->getStartTime())
            {
                upperBound++;
                if(upperBound != scheduledEvents.end())
                {
                    closestEvent = *(upperBound++);
                }
                else
                {
                    freeDuration = endTime - closestEvent->getEndTime();
                    freeStartTime = closestEvent->getEndTime();
                    found = true;
                }
            }
            
            if(!found && (closestEvent->getStartTime() <= endTime))
            {
                freeDuration = closestEvent->getStartTime() - startTime;
                freeStartTime = closestEvent->getStartTime() - freeDuration;
                found = true;
            }
        }
        
    }
    
    return found;
}

bool Scheduler::compareTasks(std::shared_ptr<Task> a, std::shared_ptr<Task> b)
{
    return (a->getLatestEndTime() < b->getLatestEndTime());
}
bool Scheduler::compareEvents(std::shared_ptr<Event> a, std::shared_ptr<Event> b)
{
    return (a->getStartTime() < b->getStartTime());
}
