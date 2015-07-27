#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <memory>

#include "task.hpp"

namespace Tracker
{
namespace Core
{
 
class Event
{
public:

	enum class Status
	{
		Logged,
		Running,
		Scheduled
	};

    Event();
    Event(unsigned int startTime, unsigned int duration);
    uint64_t getDuration() const;
    void setDuration(uint64_t duration);
    uint64_t getStartTime() const;
    uint64_t getEndTime() const;
    void setStartTime(uint64_t startTime);
    Status getStatus() const;
    void setStatus(Status type);
    void setEventId(uint64_t eventId);
    uint64_t getEventId() const;
    std::shared_ptr<const Task> getParent() const;
    void setParent(const std::shared_ptr<const Task> & parent);
    bool overlaps(const std::shared_ptr<Event> & eventToCheck) const;
private:
    uint64_t startTime;
    uint64_t duration;
    std::shared_ptr<const Task> parent;
    Status status;
    uint64_t eventId;
};
    
}
}

#endif
