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
	
	struct GetTasksProcedure : public JsonRequestProcedure
	{
		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	SayHelloProcedure sayHello;
	GetTasksProcedure getTasks;
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
