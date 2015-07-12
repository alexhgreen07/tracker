#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <memory>
#include <vector>
#include <string>

namespace Tracker
{
namespace Core
{

class Task :
    public std::enable_shared_from_this<Task>
{
public:

	enum class Status
	{
		Incomplete,
		Complete,
		Missed
	};

    Task();
    Task(std::string name, uint64_t earliestStartTime, uint64_t latestEndTime, uint64_t duration);
    
    unsigned int getTaskId() const;
    void setTaskId(unsigned int taskId);
    std::string getName() const;
    void setName(std::string name);
    uint64_t getEarliestStartTime() const;
    void setEarliestStartTime(uint64_t earliestStartTime);
    uint64_t getLatestEndTime() const;
    void setLatestEndTime(uint64_t latestEndTime);
    uint64_t getLatestStartTime() const;
    uint64_t getDuration() const;
    void setDuration(uint64_t duration);
    Status getStatus();
    void setStatus(Status status);
    void addChild(std::shared_ptr<Task> child);
    std::shared_ptr<Task> getChild(unsigned int index);
    size_t getChildrenCount();
    void removeChild(unsigned int index);
    std::weak_ptr<Task> getParent();
    
    uint64_t getRecurringPeriod() const;
    uint64_t getRecurringLateOffset() const;
    void setRecurranceParameters(uint64_t period, uint64_t lateOffset);
    size_t getRecurringTaskCount() const;
    std::weak_ptr<Task> getRecurranceParent() const;
    bool getIsRecurringParent() const;
    void clearRecurranceParameters();
    
    std::shared_ptr<const Task> getRecurringChild(unsigned int index) const;
    
protected:
    void setParent(const std::shared_ptr<Task> & parent);
    void setRecurranceParent(const std::shared_ptr<Task> & parent);
    
private:
    unsigned int taskId;
    std::string name;
    uint64_t earliestStartTime;
    uint64_t latestEndTime;
    uint64_t duration;
    std::vector<std::shared_ptr<Task>> children;
    std::vector<std::shared_ptr<Task>> recurringChildren;
    std::weak_ptr<Task> parent;
    Status status;
    
    uint64_t recurringPeriod;
    uint64_t recurringLateOffset;
    std::weak_ptr<Task> recurrenceParent;

    void validateAndCorrectData();
};
    
}
}

#endif
