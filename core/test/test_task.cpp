#include <CppUTest/TestHarness.h>

#include "task.hpp"

TEST_GROUP(TaskGroup)
{
    std::shared_ptr<Task> testTask;
    
    void setup()
    {
        testTask = std::make_shared<Task>();
    }
};

TEST(TaskGroup, BasicInitialize)
{
    LONGS_EQUAL(0,testTask->getEarliestStartTime());
    LONGS_EQUAL(0,testTask->getLatestEndTime());
    LONGS_EQUAL(0,testTask->getDuration());
    LONGS_EQUAL(0,testTask->getChildrenCount());
    CHECK(testTask->getParent().expired());
}

TEST(TaskGroup, SetEarliestStartTime)
{
    unsigned int testTime = 10;
    testTask->setEarliestStartTime(testTime);
    
    LONGS_EQUAL(testTime,testTask->getEarliestStartTime());
}

TEST(TaskGroup, SetLatestEndTime)
{
    unsigned int testTime = 15;
    testTask->setLatestEndTime(testTime);
    
    LONGS_EQUAL(testTime,testTask->getLatestEndTime());
}

TEST(TaskGroup, SetDuration)
{
    unsigned int testDuration = 20;
    testTask->setDuration(testDuration);
    
    LONGS_EQUAL(testDuration, testTask->getDuration())
}

TEST(TaskGroup, AddChild)
{
    std::shared_ptr<Task> newChild = std::make_shared<Task>();
    testTask->addChild(newChild);
    
    LONGS_EQUAL(1,testTask->getChildrenCount());
}

TEST(TaskGroup, GetChild)
{
    std::shared_ptr<Task> newChild = std::make_shared<Task>();
    testTask->addChild(newChild);
    std::shared_ptr<Task> child = testTask->getChild(0);
    
    CHECK(child.get() == newChild.get());
}

TEST(TaskGroup, RemoveChild)
{
    std::shared_ptr<Task> newChild = std::make_shared<Task>();
    testTask->addChild(newChild);
    testTask->removeChild(0);
    
    LONGS_EQUAL(0,testTask->getChildrenCount());
}

TEST(TaskGroup, GetParentofChild)
{
    std::shared_ptr<Task> newChild = std::make_shared<Task>();
    testTask->addChild(newChild);
    std::shared_ptr<Task> childParent = std::shared_ptr<Task>(newChild->getParent());
    
    CHECK(childParent.get() == testTask.get());
}

TEST(TaskGroup, CheckParentofChildAfterRemoval)
{
    std::shared_ptr<Task> newChild = std::make_shared<Task>();
    testTask->addChild(newChild);
    testTask->removeChild(0);
    std::weak_ptr<Task> childParent = newChild->getParent();
    
    CHECK(childParent.expired());
}

TEST(TaskGroup, SetRecurringParameters)
{
    const unsigned int duration = 5;
    const unsigned int period = 10;
    testTask->setLatestEndTime(duration * period);
    testTask->setDuration(duration);
    testTask->setRecurranceParameters(period,duration);
    
    LONGS_EQUAL(5,testTask->getRecurringTaskCount());
    
    for(unsigned int i = 0; i < testTask->getRecurringTaskCount(); i++)
    {
        std::shared_ptr<const Task> recurringTask = testTask->getRecurringChild(i);
        LONGS_EQUAL(i * 10,recurringTask->getEarliestStartTime());
        LONGS_EQUAL(i * 10 + 5,recurringTask->getLatestEndTime());
        LONGS_EQUAL(duration,recurringTask->getDuration());
        
        std::shared_ptr<Task> recurrenceParent = std::shared_ptr<Task>(recurringTask->getRecurranceParent());
        CHECK(testTask == recurrenceParent);
    }
}

TEST(TaskGroup, ClearRecurringParameters)
{
    const unsigned int duration = 5;
    const unsigned int period = 10;
    testTask->setLatestEndTime(duration * period);
    testTask->setDuration(duration);
    testTask->setRecurranceParameters(period,duration);
    
    testTask->clearRecurranceParameters();
    
    CHECK_FALSE(testTask->getIsRecurringParent());
}
