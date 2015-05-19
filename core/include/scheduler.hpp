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
    void setTaskList(std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList);
    size_t getScheduledEventCount();
    void schedule();
    std::shared_ptr<Event> getScheduledEvent(unsigned int index);
    
private:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    std::vector<std::shared_ptr<Event>> scheduledEvents;
    
    std::shared_ptr<std::vector<std::shared_ptr<Event>>>
        scheduleInFreeSpace(std::shared_ptr<Task> currentTask, unsigned int & earliestFreeTime);
    
    static bool compareTasks(std::shared_ptr<Task> a, std::shared_ptr<Task> b);
    static bool compareEvents(std::shared_ptr<Event> a, std::shared_ptr<Event> b);
};

#endif