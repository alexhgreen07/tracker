#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <json-rpc.h>

#include "api.hpp"

using namespace jsonrpc;

namespace Tracker
{
namespace Network
{

class Server
{
public:
    Server(const std::shared_ptr<Api> & api);
    bool start();
private:
    
	std::shared_ptr<Api> api;
    HTTPConnector http;
    JsonRpcServer serv;
};
    
}
}

#endif
