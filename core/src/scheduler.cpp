#include "scheduler.hpp"

namespace Tracker
{
namespace Core
{

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
        
        auto spaceIsAvailable = findFreeSpaceAfter(
                                                     nextStartTime,
                                                     remainingDuration,
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

bool Scheduler::findFreeSpaceAfter(unsigned int startTime, unsigned int duration, unsigned int & freeStartTime, unsigned int & freeDuration)
{
    bool found = true;
    auto newEvent = std::make_shared<Event>(startTime,duration);
    
    if(scheduledEvents.size() == 0)
    {
        freeDuration = duration;
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
                
                if(startTime < closestEvent->getEndTime())
                {
                	freeStartTime = closestEvent->getEndTime();
                }
                else
                {
                	freeStartTime = startTime;
                }

                freeDuration = duration;

            }
            else
            {
                auto nextClosestEvent = *(lowerBound);
				auto closestEvent = *(--lowerBound);
				
				if(startTime < closestEvent->getEndTime())
				{
					freeStartTime = closestEvent->getEndTime();
				}
				else
				{
					freeStartTime = startTime;
				}
				
                freeDuration = nextClosestEvent->getStartTime() - freeStartTime;

                ++lowerBound;
            }
            
            lowerBound++;
        }
        
    }
    
    return found;
}

bool getFreeSpaceBetweenEvents(std::shared_ptr<Event> & firstEvent, std::shared_ptr<Event> & secondEvent, unsigned int & duration)
{
	bool foundSpace = false;
	duration = firstEvent->getEndTime() - secondEvent->getStartTime();

	if(duration > 0)
	{
		foundSpace = true;
	}

	return foundSpace;
}

bool Scheduler::compareTasks(const std::shared_ptr<Task> & a, const std::shared_ptr<Task> & b)
{
    return (a->getLatestEndTime() < b->getLatestEndTime());
}
bool Scheduler::compareEvents(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b)
{
    return (a->getStartTime() < b->getStartTime());
}
    
}
}
