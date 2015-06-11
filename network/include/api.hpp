#ifndef __API_HPP__
#define __API_HPP__

#include <json-rpc.h>

using namespace jsonrpc;

namespace Tracker
{
namespace Network
{
namespace Api
{

struct Procedures{
	Procedures();
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};
	
void sayHello(const Json::Value& request, Json::Value& response);

void notifyServer(const Json::Value& request);

}
	
}
}

#endif