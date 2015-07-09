#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

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
    size_t getScheduledEventCount() const;
    void schedule();
    std::shared_ptr<Event> getScheduledEvent(unsigned int index) const;
    
private:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    std::vector<std::shared_ptr<Event>> scheduledEvents;
    
    std::shared_ptr<std::vector<std::shared_ptr<Event>>>
        scheduleInFreeSpace(const std::shared_ptr<const Task> & currentTask);
    
    void scheduleOneOffInFreeSpace(std::shared_ptr<std::vector<std::shared_ptr<Event>>> & scheduledEvents,
                                  const std::shared_ptr<const Task> & currentTask);
    
    bool findFreeSpaceAfter(unsigned int startTime, unsigned int duration, unsigned int & freeStartTime, unsigned int & freeDuration);
    bool getFreeSpaceBetweenEvents(std::shared_ptr<Event> & firstEvent, std::shared_ptr<Event> & secondEvent, unsigned int & duration);
    
    static bool compareTasks(const std::shared_ptr<const Task> & a, const std::shared_ptr<const Task> & b);
    static bool compareEvents(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b);
};
    
}
}

#endif
