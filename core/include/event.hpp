#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <memory>

#include "task.hpp"

class Event
{
public:
    Event();
    Event(unsigned int startTime, unsigned int duration);
    unsigned int getDuration();
    void setDuration(unsigned int duration);
    unsigned int getStartTime();
    unsigned int getEndTime();
    void setStartTime(unsigned int startTime);
    std::shared_ptr<Task> getParent();
    void setParent(const std::shared_ptr<Task> & parent);
    bool overlaps(const std::shared_ptr<Event> & eventToCheck);
private:
    unsigned int startTime;
    unsigned int duration;
    std::shared_ptr<Task> parent;
};

#endif