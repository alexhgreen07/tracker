#include "api.hpp"

namespace Tracker
{
namespace Network
{
namespace Api
{

Procedures::Procedures()
{
	procedurePointers["sayHello"] = &sayHello;
	notPointers["notifyServer"] = &notifyServer;
}
	
void SayHelloProcedure::call(const Json::Value& request, Json::Value& response) {
	response = "Hello: " + request["name"].asString();
}

void NotifyServerProcedure::call(const Json::Value& request) {
	cout << "server received some Notification" << endl;
}

}
}
}