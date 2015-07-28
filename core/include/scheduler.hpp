#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

#include <stdint.h>
#include <vector>
#include <memory>

#include "task.hpp"
#include "event.hpp"

namespace Tracker
{
namespace Core
{

class Scheduler
{
public:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> getTaskList();
    void setTaskList(const std::shared_ptr<std::vector<std::shared_ptr<Task>>> & taskList);
    void setLoggedEventList(const std::shared_ptr<std::vector<std::shared_ptr<Event>>> & eventList);
    std::shared_ptr<std::vector<std::shared_ptr<Event>>> getLoggedEventList();

    size_t getScheduledEventCount() const;
    void schedule(uint64_t minStartTime = 0);
    std::shared_ptr<Event> getScheduledEvent(unsigned int index) const;
    
private:
    struct TaskEntry
	{
    	std::shared_ptr<const Task> task;
    	uint64_t loggedDuration;
	};

    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    std::shared_ptr<std::vector<std::shared_ptr<Event>>> loggedEventList;
    std::vector<std::shared_ptr<Event>> scheduledEvents;
    
    std::shared_ptr<std::vector<std::shared_ptr<Event>>>
        scheduleInFreeSpace(const std::shared_ptr<TaskEntry> & currentTask, uint64_t minStartTime);
    
    void scheduleOneOffInFreeSpace(std::shared_ptr<std::vector<std::shared_ptr<Event>>> & scheduledEvents,
                                  const std::shared_ptr<TaskEntry> & currentTask, uint64_t minStartTime);
    
    bool findFreeSpaceAfter(unsigned int startTime, unsigned int duration, unsigned int & freeStartTime, unsigned int & freeDuration);
    bool getFreeSpaceBetweenEvents(std::shared_ptr<Event> & firstEvent, std::shared_ptr<Event> & secondEvent, unsigned int & duration);
    
    static bool compareTaskEntriesByLatestEndTime(const std::shared_ptr<TaskEntry> & a, const std::shared_ptr<TaskEntry> & b);
    static bool compareTasksByLatestEndTime(const std::shared_ptr<const Task> & a, const std::shared_ptr<const Task> & b);
    static bool compareEventsByStartTime(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b);
};
    
}
}

#endif
