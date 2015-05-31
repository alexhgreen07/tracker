#ifndef __APP_HPP__
#define __APP_HPP__

#include <server.hpp>

namespace Tracker
{
namespace Application
{

using namespace Network;
    
class TrackerApp
{
public:
    bool start();
private:
    Server server;
};
    
}
}



#endif