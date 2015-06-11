#include "api.hpp"

namespace Tracker
{
namespace Network
{

Api::Api()
{
	procedurePointers["sayHello"] = &sayHello;
	notPointers["notifyServer"] = &notifyServer;
}
	
void Api::SayHelloProcedure::call(const Json::Value& request, Json::Value& response) {
	response = "Hello: " + request["name"].asString();
}

void Api::NotifyServerProcedure::call(const Json::Value& request) {
	cout << "server received some Notification" << endl;
}

}
}