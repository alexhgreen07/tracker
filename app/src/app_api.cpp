#include "app_api.hpp"

namespace Tracker
{
namespace Application
{
	
AppApi::AppApi()
{
	procedurePointers["sayHello"] = &sayHello;
	notPointers["notifyServer"] = &notifyServer;
}

JsonMethods & AppApi::getProcedures()
{
	return procedurePointers;
}
JsonNotifications & AppApi::getNotifications()
{
	return notPointers;
}
	
void AppApi::SayHelloProcedure::call(const Json::Value& request, Json::Value& response)
{
	response = "Hello: " + request["name"].asString();
}

void AppApi::NotifyServerProcedure::call(const Json::Value& request)
{
	cout << "server received some Notification" << endl;
}
	
}
}