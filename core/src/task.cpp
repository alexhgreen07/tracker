#include "task.hpp"

#include <cstddef>

Task::Task() :
    earliestStartTime(0),
    latestEndTime(0),
    duration(0),
    children()
{}

Task::Task(unsigned int earliestStartTime,unsigned int latestEndTime,unsigned int duration) :
    earliestStartTime(earliestStartTime),
    latestEndTime(latestEndTime),
    duration(duration)
{}

unsigned int Task::getEarliestStartTime()
{
    return earliestStartTime;
}
void Task::setEarliestStartTime(unsigned int earliestStartTime)
{
    this->earliestStartTime = earliestStartTime;
}
unsigned int Task::getLatestEndTime()
{
    return latestEndTime;
}
void Task::setLatestEndTime(unsigned int latestEndTime)
{
    this->latestEndTime = latestEndTime;
}
unsigned int Task::getLatestStartTime()
{
    return latestEndTime - duration;
}
unsigned int Task::getDuration()
{
    return duration;
}
void Task::setDuration(unsigned int duration)
{
    this->duration = duration;
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
void Task::setParent(const std::shared_ptr<Task> & parent)
{
    this->parent = parent;
}