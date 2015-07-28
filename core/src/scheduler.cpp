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
void Scheduler::setLoggedEventList(const std::shared_ptr<std::vector<std::shared_ptr<Event>>> & eventList)
{
	loggedEventList = eventList;
}
std::shared_ptr<std::vector<std::shared_ptr<Event>>> Scheduler::getLoggedEventList()
{
	return loggedEventList;
}
size_t Scheduler::getScheduledEventCount() const
{
    return scheduledEvents.size();
}
void Scheduler::schedule(uint64_t minStartTime)
{
    if(taskList)
    {
    	auto taskEntriesToSchedule = std::make_shared<std::vector<std::shared_ptr<TaskEntry>>>();
    	for(auto iter = taskList->begin(); iter != taskList->end(); iter++)
    	{
    		auto taskToCheck = *iter;

    		if(taskToCheck->getStatus() == Task::Status::Incomplete)
    		{
    			if(taskToCheck->getRecurringTaskCount() == 0)
				{
    				auto newEntry = std::make_shared<TaskEntry>();
    				newEntry->task = taskToCheck;
    				newEntry->loggedDuration = 0;
					taskEntriesToSchedule->push_back(newEntry);
				}
				else
				{
					for(unsigned int j = 0; j < taskToCheck->getRecurringTaskCount(); j++)
					{
						auto taskToAdd = taskToCheck->getRecurringChild(j);
						auto newEntry = std::make_shared<TaskEntry>();
						newEntry->task = taskToAdd;
						newEntry->loggedDuration = 0;
						taskEntriesToSchedule->push_back(newEntry);
					}
				}
    		}
    	}

    	if(loggedEventList)
    	{
    		//TODO: make this nested loop more efficient
    		for(auto taskIter = taskEntriesToSchedule->begin(); taskIter != taskEntriesToSchedule->end(); taskIter++)
			{
				auto currentEntry = *taskIter;

				for(auto eventIter = loggedEventList->begin(); eventIter != loggedEventList->end(); eventIter++)
				{
					auto loggedEvent = *eventIter;
					if(loggedEvent->getParent() == currentEntry->task)
					{
						currentEntry->loggedDuration += loggedEvent->getDuration();
					}
				}
			}
    	}

        std::sort(taskEntriesToSchedule->begin(),taskEntriesToSchedule->end(),compareTaskEntriesByLatestEndTime);
        scheduledEvents.clear();
        
        for(auto taskIter = taskEntriesToSchedule->begin(); taskIter != taskEntriesToSchedule->end(); taskIter++)
        {
            auto currentEntry = *taskIter;
            auto newEvents = scheduleInFreeSpace(currentEntry,minStartTime);
            
            for(auto eventIter = newEvents->begin(); eventIter != newEvents->end(); eventIter++)
            {
                auto newEvent = *eventIter;
                auto upperBound =
                    std::upper_bound(scheduledEvents.begin(),scheduledEvents.end(),newEvent,compareEventsByStartTime);
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
    Scheduler::scheduleInFreeSpace(const std::shared_ptr<TaskEntry> & currentEntry, uint64_t minStartTime)
{
    auto scheduledEvents = std::make_shared<std::vector<std::shared_ptr<Event>>>();
    
    scheduleOneOffInFreeSpace(scheduledEvents,currentEntry,minStartTime);
    
    return scheduledEvents;
}

void Scheduler::scheduleOneOffInFreeSpace(std::shared_ptr<std::vector<std::shared_ptr<Event>>> & scheduledEvents, const std::shared_ptr<TaskEntry> & currentEntry, uint64_t minStartTime)
{
    unsigned int remainingDuration = currentEntry->task->getDuration();
    unsigned int nextStartTime = currentEntry->task->getEarliestStartTime();
    
    if(remainingDuration >= currentEntry->loggedDuration)
    {
    	remainingDuration -= currentEntry->loggedDuration;
    }
    else
    {
    	remainingDuration = 0;
    }

    if(nextStartTime < minStartTime)
    {
    	nextStartTime = minStartTime;
    }

    while(remainingDuration > 0)
    {
        auto newEvent = std::make_shared<Event>();
        unsigned int freeStartTime = 0;
        unsigned int freeDuration = 0;
        
        if(currentEntry->task->getRecurranceParent().expired())
        {
        	newEvent->setParent(currentEntry->task);
        }
        else
        {
        	auto parent = std::shared_ptr<Task>(currentEntry->task->getRecurranceParent());
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
            std::lower_bound(scheduledEvents.begin(),scheduledEvents.end(),newEvent,compareEventsByStartTime);
        
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

bool Scheduler::compareTaskEntriesByLatestEndTime(const std::shared_ptr<TaskEntry> & a, const std::shared_ptr<TaskEntry> & b)
{
	return compareTasksByLatestEndTime(a->task,b->task);
}

bool Scheduler::compareTasksByLatestEndTime(const std::shared_ptr<const Task> & a, const std::shared_ptr<const Task> & b)
{
    return (a->getLatestEndTime() < b->getLatestEndTime());
}
bool Scheduler::compareEventsByStartTime(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b)
{
    return (a->getStartTime() < b->getStartTime());
}
    
}
}
