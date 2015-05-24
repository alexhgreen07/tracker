#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <memory>
#include <vector>

class Task :
    public std::enable_shared_from_this<Task>
{
public:
    Task();
    Task(unsigned int earliestStartTime,unsigned int latestEndTime,unsigned int duration);
    
    unsigned int getEarliestStartTime() const;
    void setEarliestStartTime(unsigned int earliestStartTime);
    unsigned int getLatestEndTime() const;
    void setLatestEndTime(unsigned int latestEndTime);
    unsigned int getLatestStartTime() const;
    unsigned int getDuration() const;
    void setDuration(unsigned int duration);
    void addChild(std::shared_ptr<Task> child);
    std::shared_ptr<Task> getChild(unsigned int index);
    size_t getChildrenCount();
    void removeChild(unsigned int index);
    std::weak_ptr<Task> getParent();
    
    void setRecurranceParameters(unsigned int period, unsigned int lateOffset);
    size_t getRecurringTaskCount();
    void clearRecurranceParameters();
    
    std::shared_ptr<const Task> getRecurringChild(unsigned int index);
    
protected:
    void setParent(const std::shared_ptr<Task> & parent);
    
private:
    unsigned int earliestStartTime;
    unsigned int latestEndTime;
    unsigned int duration;
    std::vector<std::shared_ptr<Task>> children;
    std::vector<std::shared_ptr<Task>> recurringChildren;
    std::weak_ptr<Task> parent;
    
    unsigned int recurringPeriod;
    unsigned int recurringLateOffset;
};

#endif