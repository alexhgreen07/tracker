#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

#include <vector>
#include <memory>

#include "task.hpp"
#include "event.hpp"

class Scheduler
{
public:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> getTaskList()
    {
        return std::shared_ptr<std::vector<std::shared_ptr<Task>>>(nullptr);
    }
    unsigned int getScheduledEventCount()
    {
        return 0;
    }
private:
    
};

TEST_GROUP(SchedulerGroup)
{
    std::shared_ptr<Scheduler> testScheduler;
    
    void setup()
    {
        testScheduler = std::make_shared<Scheduler>();
    }
};

TEST(SchedulerGroup, BasicInitialize)
{
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    
    taskList = testScheduler->getTaskList();
    CHECK(!taskList);
    LONGS_EQUAL(0, testScheduler->getScheduledEventCount());
}

#endif