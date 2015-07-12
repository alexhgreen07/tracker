#include "task.hpp"

#include <cstddef>

namespace Tracker
{
namespace Core
{

Task::Task() :
	taskId(0),
	name(""),
    earliestStartTime(0),
    latestEndTime(0),
    duration(0),
    children(),
    recurringPeriod(0),
    recurringLateOffset(0),
	status(Status::Incomplete)
{}

Task::Task(std::string name, uint64_t earliestStartTime, uint64_t latestEndTime, uint64_t duration) :
	taskId(0),
	name(name),
    earliestStartTime(earliestStartTime),
    latestEndTime(latestEndTime),
    duration(duration),
    children(),
    recurringPeriod(0),
    recurringLateOffset(0),
	status(Status::Incomplete)
{
	validateAndCorrectData();
}

void Task::setTaskId(unsigned int taskId)
{
	this->taskId = taskId;
}

unsigned int Task::getTaskId() const
{
	return taskId;
}

std::string Task::getName() const
{
	return name;
}

void Task::setName(std::string name)
{
	this->name = name;
}

uint64_t Task::getEarliestStartTime() const
{
    return earliestStartTime;
}
void Task::setEarliestStartTime(uint64_t earliestStartTime)
{
    this->earliestStartTime = earliestStartTime;
    validateAndCorrectData();
}
uint64_t Task::getLatestEndTime() const
{
    return latestEndTime;
}
void Task::setLatestEndTime(uint64_t latestEndTime)
{
    this->latestEndTime = latestEndTime;
    validateAndCorrectData();
}
uint64_t Task::getLatestStartTime() const
{
    return latestEndTime - duration;
}
uint64_t Task::getDuration() const
{
    return duration;
}
void Task::setDuration(uint64_t duration)
{
    this->duration = duration;
    validateAndCorrectData();
}
Task::Status Task::getStatus()
{
	return status;
}
void Task::setStatus(Status status)
{
	this->status = status;
}
void Task::addChild(std::shared_ptr<Task> child)
{
    //TODO: ensure checks are made for parent/child compatibility
    
    child->setParent(shared_from_this());
    children.push_back(child);
}
std::shared_ptr<Task> Task::getChild(unsigned int index)
{
    return children[index];
}
size_t Task::getChildrenCount()
{
    return children.size();
}
void Task::removeChild(unsigned int index)
{
    std::shared_ptr<Task> childToErase = children[index];
    
    children.erase(children.begin() + index);
    
    childToErase->setParent(std::shared_ptr<Task>(nullptr));
}
std::weak_ptr<Task> Task::getParent()
{
    return parent;
}

uint64_t Task::getRecurringPeriod() const
{
	return recurringPeriod;
}

uint64_t Task::getRecurringLateOffset() const
{
	return recurringLateOffset;
}

void Task::setRecurranceParameters(uint64_t period, uint64_t lateOffset)
{
    recurringPeriod = period;
    recurringLateOffset = lateOffset;
    
    recurringChildren.clear();
    
    if(recurringPeriod > 0)
    {
        for(unsigned int i = 0; i < (latestEndTime - earliestStartTime) / recurringPeriod; i++)
        {
            unsigned int childEarliestStartTime = earliestStartTime + i * recurringPeriod;
            unsigned int childLatestEndTime = earliestStartTime + recurringLateOffset + i * recurringPeriod;
            auto newChild = std::make_shared<Task>(name,childEarliestStartTime,childLatestEndTime,duration);
            newChild->setRecurranceParent(shared_from_this());
            recurringChildren.push_back(newChild);
        }
    }
}

size_t Task::getRecurringTaskCount() const
{
    return recurringChildren.size();
}

std::shared_ptr<const Task> Task::getRecurringChild(unsigned int index) const
{
    std::shared_ptr<const Task> returnValue = recurringChildren[index];
    return returnValue;
}

std::weak_ptr<Task> Task::getRecurranceParent() const
{
    return recurrenceParent;
}

bool Task::getIsRecurringParent() const
{
    return (recurringChildren.size() > 0);
}

void Task::clearRecurranceParameters()
{
    setRecurranceParameters(0,0);
}

void Task::setParent(const std::shared_ptr<Task> & parent)
{
    this->parent = parent;
}

void Task::setRecurranceParent(const std::shared_ptr<Task> & parent)
{
    this->recurrenceParent = parent;
}

void Task::validateAndCorrectData()
{
	if(earliestStartTime > getLatestStartTime())
	{
		latestEndTime = earliestStartTime + duration;
	}
}
    
}
}
