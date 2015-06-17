#ifndef __APP_HPP__
#define __APP_HPP__

#include <server.hpp>
#include <app_api.hpp>

namespace Tracker
{
namespace Application
{

using namespace Network;
    
class TrackerApp
{
public:
	TrackerApp();
    bool start();
private:
	AppApi api;
    Server server;
};
    
}
}



#endif