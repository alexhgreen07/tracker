#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <memory>

#include "task.hpp"

class Event
{
public:
    Event();
    Event(unsigned int startTime, unsigned int duration);
    unsigned int getDuration() const;
    void setDuration(unsigned int duration);
    unsigned int getStartTime() const;
    unsigned int getEndTime() const;
    void setStartTime(unsigned int startTime);
    std::shared_ptr<const Task> getParent() const;
    void setParent(const std::shared_ptr<const Task> & parent);
    bool overlaps(const std::shared_ptr<Event> & eventToCheck) const;
private:
    unsigned int startTime;
    unsigned int duration;
    std::shared_ptr<const Task> parent;
};

#endif