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
    
    unsigned int getEarliestStartTime();
    void setEarliestStartTime(unsigned int earliestStartTime);
    unsigned int getLatestEndTime();
    void setLatestEndTime(unsigned int latestEndTime);
    unsigned int getLatestStartTime();
    unsigned int getDuration();
    void setDuration(unsigned int duration);
    void addChild(std::shared_ptr<Task> child);
    std::shared_ptr<Task> getChild(unsigned int index);
    size_t getChildrenCount();
    void removeChild(unsigned int index);
    std::weak_ptr<Task> getParent();
    
protected:
    void setParent(std::shared_ptr<Task> parent);
    
private:
    unsigned int earliestStartTime;
    unsigned int latestEndTime;
    unsigned int duration;
    std::vector<std::shared_ptr<Task>> children;
    std::weak_ptr<Task> parent;
};

#endif