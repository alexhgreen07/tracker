#ifndef __API_HPP__
#define __API_HPP__

#include <json-rpc.h>

using namespace jsonrpc;

namespace Tracker
{
namespace Network
{
	
class Api
{
public:
	Api();
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
protected:
	struct SayHelloProcedure : public JsonRequestProcedure
	{
		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	struct NotifyServerProcedure : public JsonNotificationProcedure
	{
		void call(const Json::Value& request) override;
	};
	
	SayHelloProcedure sayHello;
	NotifyServerProcedure notifyServer;
};
	
}
}

#endif