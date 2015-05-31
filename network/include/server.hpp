#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <json-rpc.h>

using namespace jsonrpc;

namespace Tracker
{
namespace Network
{

class Server
{
public:
    Server();
    bool start();
private:
    struct Procedures{
        Procedures();
        JsonMethods procedurePointers;
        JsonNotifications notPointers;
    };
    
    Procedures procedures;
    HTTPConnector http;
    JsonRpcServer serv;
};
    
}
}

#endif