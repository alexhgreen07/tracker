#include <api.hpp>

namespace Tracker
{
namespace Application
{
using namespace Network;

class AppApi : public Api
{
public:
	AppApi()
	{
		procedurePointers["sayHello"] = &sayHello;
		notPointers["notifyServer"] = &notifyServer;
	}
	
	JsonMethods & getProcedures()
	{
		return procedurePointers;
	}
	JsonNotifications & getNotifications()
	{
		return notPointers;
	}
protected:
	struct SayHelloProcedure : public JsonRequestProcedure
	{
		void call(const Json::Value& request, Json::Value& response) override
		{
			response = "Hello: " + request["name"].asString();
		}
	};
	
	struct NotifyServerProcedure : public JsonNotificationProcedure
	{
		void call(const Json::Value& request) override
		{
			cout << "server received some Notification" << endl;
		}
	};
	
	SayHelloProcedure sayHello;
	NotifyServerProcedure notifyServer;
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
