#include <api.hpp>

namespace Tracker
{
namespace Application
{
using namespace Network;

class AppApi : public Api
{
public:
	AppApi();
	
	JsonMethods & getProcedures();
	JsonNotifications & getNotifications();
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
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
