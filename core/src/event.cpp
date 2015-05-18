#include "event.hpp"

#include <cstddef>

Event::Event() :
    earliestStartTime(0),
    latestEndTime(0),
    duration(0),
    children()
{}

unsigned int Event::getEarliestStartTime()
{
    return earliestStartTime;
}
void Event::setEarliestStartTime(unsigned int earliestStartTime)
{
    this->earliestStartTime = earliestStartTime;
}
unsigned int Event::getLatestEndTime()
{
    return latestEndTime;
}
void Event::setLatestEndTime(unsigned int latestEndTime)
{
    this->latestEndTime = latestEndTime;
}
unsigned int Event::getDuration()
{
    return duration;
}
void Event::setDuration(unsigned int duration)
{
    this->duration = duration;
}
void Event::addChild(std::shared_ptr<Event> child)
{
    //TODO: ensure checks are made for parent/child compatibility
    
    child->setParent(shared_from_this());
    children.push_back(child);
}
std::shared_ptr<Event> Event::getChild(unsigned int index)
{
    return children[index];
}
size_t Event::getChildrenCount()
{
    return children.size();
}
void Event::removeChild(unsigned int index)
{
    std::shared_ptr<Event> childToErase = children[index];
    
    children.erase(children.begin() + index);
    
    childToErase->setParent(std::shared_ptr<Event>(nullptr));
}
std::weak_ptr<Event> Event::getParent()
{
    return parent;
}
void Event::setParent(std::shared_ptr<Event> parent)
{
    this->parent = parent;
}