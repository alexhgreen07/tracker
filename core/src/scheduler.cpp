#include "scheduler.hpp"

std::shared_ptr<std::vector<std::shared_ptr<Task>>> Scheduler::getTaskList()
{
    return taskList;
}
void Scheduler::setTaskList(const std::shared_ptr<std::vector<std::shared_ptr<Task>>> & taskList)
{
    this->taskList = taskList;
}
size_t Scheduler::getScheduledEventCount() const
{
    return scheduledEvents.size();
}
void Scheduler::schedule()
{
    if(taskList)
    {
        std::sort(taskList->begin(),taskList->end(),compareTasks);
        scheduledEvents.clear();
        
        for(unsigned int i = 0; i < taskList->size(); i++)
        {
            auto currentTask = taskList->at(i);
            auto newEvents = scheduleInFreeSpace(currentTask);
            
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
std::shared_ptr<Event> Scheduler::getScheduledEvent(unsigned int index) const
{
    return scheduledEvents[index];
}

std::shared_ptr<std::vector<std::shared_ptr<Event>>>
    Scheduler::scheduleInFreeSpace(const std::shared_ptr<Task> & currentTask)
{
    auto scheduledEvents = std::make_shared<std::vector<std::shared_ptr<Event>>>();
    
    if(currentTask->getIsRecurringParent())
    {
        for(unsigned int i = 0 ; i < currentTask->getRecurringTaskCount(); i++)
        {
            auto recurringChild = currentTask->getRecurringChild(i);
            scheduleOneOffInFreeSpace(scheduledEvents,recurringChild);
        }
    }
    else
    {
        scheduleOneOffInFreeSpace(scheduledEvents,currentTask);
    }
    
    return scheduledEvents;
}

void Scheduler::scheduleOneOffInFreeSpace(std::shared_ptr<std::vector<std::shared_ptr<Event>>> & scheduledEvents, const std::shared_ptr<const Task> & currentTask)
{
    unsigned int remainingDuration = currentTask->getDuration();
    unsigned int nextStartTime = currentTask->getEarliestStartTime();
    
    while(remainingDuration > 0)
    {
        auto newEvent = std::make_shared<Event>();
        unsigned int freeStartTime = 0;
        unsigned int freeDuration = 0;
        
        newEvent->setParent(currentTask);
        
        auto spaceIsAvailable = findFreeSpaceBetween(
                                                     nextStartTime,
                                                     currentTask->getLatestEndTime(),
                                                     freeStartTime,freeDuration);
        
        if(!spaceIsAvailable)
        {
            freeDuration = remainingDuration;
        }
        
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
        nextStartTime = newEvent->getEndTime();
    }
    
}

bool Scheduler::findFreeSpaceBetween(unsigned int startTime, unsigned int endTime, unsigned int & freeStartTime, unsigned int & freeDuration)
{
    bool found = true;
    auto newEvent = std::make_shared<Event>(startTime,(endTime - startTime));
    
    if(scheduledEvents.size() == 0)
    {
        freeDuration = endTime - startTime;
        freeStartTime = startTime;
    }
    else
    {
        auto lowerBound =
            std::lower_bound(scheduledEvents.begin(),scheduledEvents.end(),newEvent,compareEvents);
        
        freeDuration = 0;
        while(freeDuration == 0)
        {
            if(lowerBound == scheduledEvents.begin())
            {
                auto closestEvent = *lowerBound;
                
                if(startTime < closestEvent->getStartTime())
                {
                    freeDuration = closestEvent->getStartTime() - startTime;
                    freeStartTime = closestEvent->getStartTime() - freeDuration;
                }
            }
            else if(lowerBound == scheduledEvents.end())
            {
                lowerBound--;
                auto closestEvent = *lowerBound;
                
                if((closestEvent->getEndTime() < endTime) && (closestEvent->getEndTime() >= startTime))
                {
                    freeDuration = endTime - closestEvent->getEndTime();
                    freeStartTime = closestEvent->getEndTime();
                }
                else
                {
                    if(closestEvent->getEndTime() >= startTime)
                    {
                        freeStartTime = closestEvent->getEndTime();
                    }
                    else
                    {
                        freeStartTime = startTime;                        
                    }
                    
                    found = false;
                    break;
                }
            }
            else
            {
                auto closestEvent = *lowerBound;
                auto nextClosestEvent = *(++lowerBound);
                
                freeDuration = closestEvent->getEndTime() - nextClosestEvent->getStartTime();
                freeStartTime = closestEvent->getEndTime();
            }
            
            lowerBound++;
        }
        
    }
    
    return found;
}

bool Scheduler::compareTasks(const std::shared_ptr<Task> & a, const std::shared_ptr<Task> & b)
{
    return (a->getLatestEndTime() < b->getLatestEndTime());
}
bool Scheduler::compareEvents(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b)
{
    return (a->getStartTime() < b->getStartTime());
}
