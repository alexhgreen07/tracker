#include "event.hpp"

namespace Tracker
{
namespace Core
{

Event::Event() :
    startTime(0),
    duration(0),
	status(Status::Scheduled)
{}

Event::Event(unsigned int startTime, unsigned int duration) :
    startTime(startTime),
    duration(duration),
	status(Status::Scheduled)
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

Event::Status Event::getStatus()
{
	return status;
}

void Event::setStatus(Status status)
{
	this->status = status;
}

void Event::setEventId(uint64_t eventId)
{
	this->eventId = eventId;
}

uint64_t Event::getEventId()
{
	return eventId;
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
    
}
}
