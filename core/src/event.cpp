#include "event.hpp"

namespace Tracker
{
namespace Core
{

Event::Event() :
    startTime(0),
    duration(0),
	status(Status::Scheduled),
	eventId(0)
{}

Event::Event(unsigned int startTime, unsigned int duration) :
    startTime(startTime),
    duration(duration),
	status(Status::Scheduled),
	eventId(0)
{}

uint64_t Event::getDuration() const
{
    return duration;
}

void Event::setDuration(uint64_t duration)
{
    this->duration = duration;
}

uint64_t Event::getStartTime() const
{
    return startTime;
}

uint64_t Event::getEndTime() const
{
    return startTime + duration;
}

void Event::setStartTime(uint64_t startTime)
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

uint64_t Event::getEventId() const
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
