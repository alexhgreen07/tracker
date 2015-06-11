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

	
class SayHelloProcedure : public JsonRequestProcedure
{
public:
	void call(const Json::Value& request, Json::Value& response) override;
};

class NotifyServerProcedure : public JsonNotificationProcedure
{
public:
	void call(const Json::Value& request) override;
};
	
struct Procedures{
	Procedures();
	
	SayHelloProcedure sayHello;
	NotifyServerProcedure notifyServer;
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};


}
	
}
}

#endif