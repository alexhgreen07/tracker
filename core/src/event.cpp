#include "event.hpp"

Event::Event() :
    startTime(0),
    duration(0)
{}

Event::Event(unsigned int startTime, unsigned int duration) :
    startTime(startTime),
    duration(duration)
{}

unsigned int Event::getDuration() const
{
    return duration;
}

void Event::setDuration(unsigned int duration)
{
    this->duration = duration;
}

unsigned int Event::getStartTime() const
{
    return startTime;
}

unsigned int Event::getEndTime() const
{
    return startTime + duration;
}

void Event::setStartTime(unsigned int startTime)
{
    this->startTime = startTime;
}

std::shared_ptr<const Task> Event::getParent() const
{
    return parent;
}

void Event::setParent(const std::shared_ptr<const Task> & parent)
{
    this->parent = parent;
}

bool Event::overlaps(const std::shared_ptr<Event> & eventToCheck) const
{
    return
        ((eventToCheck->getStartTime() >= startTime) &&
       (eventToCheck->getStartTime() < getEndTime())) ||
       ((eventToCheck->getEndTime() > startTime) &&
        (eventToCheck->getEndTime() < getEndTime()));
}
