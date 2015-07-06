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
    recurringLateOffset(0)
{}

Task::Task(std::string name, unsigned int earliestStartTime,unsigned int latestEndTime,unsigned int duration) :
	taskId(0),
	name(name),
    earliestStartTime(earliestStartTime),
    latestEndTime(latestEndTime),
    duration(duration)
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

unsigned int Task::getEarliestStartTime() const
{
    return earliestStartTime;
}
void Task::setEarliestStartTime(unsigned int earliestStartTime)
{
    this->earliestStartTime = earliestStartTime;
    validateAndCorrectData();
}
unsigned int Task::getLatestEndTime() const
{
    return latestEndTime;
}
void Task::setLatestEndTime(unsigned int latestEndTime)
{
    this->latestEndTime = latestEndTime;
    validateAndCorrectData();
}
unsigned int Task::getLatestStartTime() const
{
    return latestEndTime - duration;
}
unsigned int Task::getDuration() const
{
    return duration;
}
void Task::setDuration(unsigned int duration)
{
    this->duration = duration;
    validateAndCorrectData();
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

void Task::setRecurranceParameters(unsigned int period, unsigned int lateOffset)
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
