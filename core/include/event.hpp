#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <memory>

#include "task.hpp"

class Event
{
public:
    Event();
    unsigned int getDuration();
    void setDuration(unsigned int duration);
    unsigned int getStartTime();
    void setStartTime(unsigned int startTime);
    std::shared_ptr<Task> getParent();
    void setParent(std::shared_ptr<Task> parent);
private:
    unsigned int startTime;
    unsigned int duration;
    std::shared_ptr<Task> parent;
};

#endif