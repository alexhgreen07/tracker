#include "event.hpp"

Event::Event() :
    startTime(0),
    duration(0)
{}

Event::Event(unsigned int startTime, unsigned int duration) :
    startTime(startTime),
    duration(duration)
{}

unsigned int Event::getDuration()
{
    return duration;
}

void Event::setDuration(unsigned int duration)
{
    this->duration = duration;
}

unsigned int Event::getStartTime()
{
    return startTime;
}

unsigned int Event::getEndTime()
{
    return startTime + duration;
}

void Event::setStartTime(unsigned int startTime)
{
    this->startTime = startTime;
}

std::shared_ptr<Task> Event::getParent()
{
    return parent;
}

void Event::setParent(std::shared_ptr<Task> parent)
{
    this->parent = parent;
}

bool Event::overlaps(std::shared_ptr<Event> eventToCheck)
{
    return
        ((eventToCheck->getStartTime() >= startTime) &&
       (eventToCheck->getStartTime() < getEndTime())) ||
       ((eventToCheck->getEndTime() >= startTime) &&
        (eventToCheck->getEndTime() < getEndTime()));
}
