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
void Scheduler::schedule(uint64_t minStartTime)
{
    if(taskList)
    {
    	auto taskListToSchedule = std::make_shared<std::vector<std::shared_ptr<const Task>>>();
    	for(unsigned int i = 0; i < taskList->size(); i++)
    	{
    		auto taskToCheck = taskList->at(i);

    		if(taskToCheck->getRecurringTaskCount() == 0)
    		{
    			taskListToSchedule->push_back(taskToCheck);
    		}
    		else
    		{
    			for(unsigned int j = 0; j < taskToCheck->getRecurringTaskCount(); j++)
    			{
    				auto taskToAdd = taskToCheck->getRecurringChild(j);
    				taskListToSchedule->push_back(taskToAdd);
    			}
    		}

    	}

        std::sort(taskListToSchedule->begin(),taskListToSchedule->end(),compareTasks);
        scheduledEvents.clear();
        
        for(unsigned int i = 0; i < taskListToSchedule->size(); i++)
        {
            auto currentTask = taskListToSchedule->at(i);
            auto newEvents = scheduleInFreeSpace(currentTask,minStartTime);
            
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
    Scheduler::scheduleInFreeSpace(const std::shared_ptr<const Task> & currentTask, uint64_t minStartTime)
{
    auto scheduledEvents = std::make_shared<std::vector<std::shared_ptr<Event>>>();
    
    scheduleOneOffInFreeSpace(scheduledEvents,currentTask,minStartTime);
    
    return scheduledEvents;
}

void Scheduler::scheduleOneOffInFreeSpace(std::shared_ptr<std::vector<std::shared_ptr<Event>>> & scheduledEvents, const std::shared_ptr<const Task> & currentTask, uint64_t minStartTime)
{
    unsigned int remainingDuration = currentTask->getDuration();
    unsigned int nextStartTime = currentTask->getEarliestStartTime();
    
    if(nextStartTime < minStartTime)
    {
    	nextStartTime = minStartTime;
    }

    while(remainingDuration > 0)
    {
        auto newEvent = std::make_shared<Event>();
        unsigned int freeStartTime = 0;
        unsigned int freeDuration = 0;
        
        if(currentTask->getRecurranceParent().expired())
        {
        	newEvent->setParent(currentTask);
        }
        else
        {
        	auto parent = std::shared_ptr<Task>(currentTask->getRecurranceParent());
        	newEvent->setParent(parent);
        }
        
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

bool Scheduler::compareTasks(const std::shared_ptr<const Task> & a, const std::shared_ptr<const Task> & b)
{
    return (a->getLatestEndTime() < b->getLatestEndTime());
}
bool Scheduler::compareEvents(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b)
{
    return (a->getStartTime() < b->getStartTime());
}
    
}
}
