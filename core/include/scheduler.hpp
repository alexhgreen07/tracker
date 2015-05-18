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
        return taskList;
    }
    void setTaskList(std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList)
    {
        this->taskList = taskList;
    }
    unsigned int getScheduledEventCount()
    {
        return 0;
    }
private:
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
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

TEST(SchedulerGroup, SetTaskList)
{
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> taskList;
    std::shared_ptr<std::vector<std::shared_ptr<Task>>> newTaskList =
        std::make_shared<std::vector<std::shared_ptr<Task>>>();
    
    testScheduler->setTaskList(newTaskList);
    
    taskList = testScheduler->getTaskList();
    CHECK(taskList.get() == newTaskList.get());
}

#endif