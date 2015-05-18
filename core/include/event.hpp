#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <memory>
#include <vector>

class Event :
    public std::enable_shared_from_this<Event>
{
public:
    Event();
    
    unsigned int getEarliestStartTime();
    void setEarliestStartTime(unsigned int earliestStartTime);
    unsigned int getLatestEndTime();
    void setLatestEndTime(unsigned int latestEndTime);
    unsigned int getDuration();
    void setDuration(unsigned int duration);
    void addChild(std::shared_ptr<Event> child);
    std::shared_ptr<Event> getChild(unsigned int index);
    size_t getChildrenCount();
    void removeChild(unsigned int index);
    std::weak_ptr<Event> getParent();
    
protected:
    void setParent(std::shared_ptr<Event> parent);
    
private:
    unsigned int earliestStartTime;
    unsigned int latestEndTime;
    unsigned int duration;
    std::vector<std::shared_ptr<Event>> children;
    std::weak_ptr<Event> parent;
};

#endif