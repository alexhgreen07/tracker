#include "event.hpp"

namespace Tracker
{
namespace Core
{

Event::Event() :
    startTime(0),
    duration(0),
	status(Status::Scheduled),
	eventId(0),
	isLate(false)
{}

Event::Event(unsigned int startTime, unsigned int duration) :
    startTime(startTime),
    duration(duration),
	status(Status::Scheduled),
	eventId(0),
	isLate(false)
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

Event::Status Event::getStatus() const
{
	return status;
}

void Event::setStatus(Status status)
{
	this->status = status;
}

bool Event::getIsLate() const
{
	return isLate;
}

void Event::setIsLate(bool isLate)
{
	this->isLate = isLate;
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
	return overlaps(eventToCheck->getStartTime(),
			eventToCheck->getEndTime());
}

bool Event::overlaps(uint64_t startTimeToCheck, uint64_t endTimeToCheck) const
{
	bool overlapsStart = (startTimeToCheck >= startTime) &&
			   (startTimeToCheck < getEndTime());
	bool overlapsEnd = (endTimeToCheck > startTime) &&
			(endTimeToCheck < getEndTime());
	bool overlapsBetween = (startTimeToCheck < startTime)
			&& (endTimeToCheck > getEndTime());

	return (overlapsStart || overlapsEnd || overlapsBetween);
}
    
}
}
