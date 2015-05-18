#include "event.hpp"

Event::Event() :
    startTime(0),
    duration(0)
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
