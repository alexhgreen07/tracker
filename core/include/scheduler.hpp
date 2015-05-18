#ifndef __SCHEDULER_HPP__
#define __SCHEDULER_HPP__

class Scheduler
{
    
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
    
}

#endif