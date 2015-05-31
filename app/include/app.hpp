#ifndef __APP_HPP__
#define __APP_HPP__

#include <server.hpp>

class TrackerApp
{
public:
    void run();
private:
    Server server;
};

#endif