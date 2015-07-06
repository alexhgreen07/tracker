#include <api.hpp>

#include "app_db.hpp"
#include <scheduler.hpp>

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
	struct ExitProcedure : public JsonRequestProcedure
	{
		ExitProcedure()
		{}

		void call(const Json::Value& request, Json::Value& response) override;
	};

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
		
		void fillJsonValueFromTask(Json::Value& row, const Core::Task & task);

		AppApi & parent;
	};
	
	struct InsertTask : public JsonRequestProcedure
	{
		InsertTask(AppApi & parent) :
			parent(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		AppApi & parent;
	};
	
	struct UpdateTask : public JsonRequestProcedure
	{
		UpdateTask(AppApi & parent) :
			parent(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		AppApi & parent;
	};
	
	struct RemoveTask : public JsonRequestProcedure
	{
		RemoveTask(AppApi & parent) :
			parent(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		AppApi & parent;
	};
	
	struct GetEvents : public JsonRequestProcedure
	{
		GetEvents(AppApi & parent) :
			parent(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		AppApi & parent;
	};
	
	AppDB & db;
	Core::Scheduler scheduler;
	
	ExitProcedure exitProcedure;
	SayHelloProcedure sayHello;
	GetTasksProcedure getTasks;
	InsertTask insertTask;
	UpdateTask updateTask;
	RemoveTask removeTask;
	GetEvents getEvents;
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
