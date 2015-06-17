#include <api.hpp>

#include "app_db.hpp"

namespace Tracker
{
namespace Application
{
using namespace Network;

class AppApi : public Api
{
public:
	AppApi(AppDB & db);
	
	JsonMethods & getProcedures();
	JsonNotifications & getNotifications();
protected:
	struct SayHelloProcedure : public JsonRequestProcedure
	{
		SayHelloProcedure(AppApi & parent) :
			parent(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		AppApi & parent;
	};
	
	struct GetTasksProcedure : public JsonRequestProcedure
	{
		GetTasksProcedure(AppApi & parent) :
			parent(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		AppApi & parent;
	};
	
	AppDB & db;
	
	SayHelloProcedure sayHello;
	GetTasksProcedure getTasks;
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
