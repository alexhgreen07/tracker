#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <json-rpc.h>

using namespace jsonrpc;

class Server
{
public:
    Server();
    bool start();
private:
    JsonMethods procedurePointers;
    JsonNotifications notPointers;
    HTTPConnector http;
    JsonRpcServer serv;
};

#endif