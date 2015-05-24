#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

#include <vector>
#include <memory>

#include "task.hpp"
#include "event.hpp"

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
        scheduleInFreeSpace(const std::shared_ptr<Task> & currentTask, unsigned int & earliestFreeTime);
    
    bool findFreeSpaceBetween(unsigned int startTime, unsigned int endTime, unsigned int & freeStartTime, unsigned int & duration);
    
    static bool compareTasks(const std::shared_ptr<Task> & a, const std::shared_ptr<Task> & b);
    static bool compareEvents(const std::shared_ptr<Event> & a, const std::shared_ptr<Event> & b);
};

#endif