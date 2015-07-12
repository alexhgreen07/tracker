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
	
	class Clock
	{
	public:
		virtual uint64_t getNowTimestamp() = 0;
	};

	AppApi(const std::shared_ptr<AppDB> & db, const std::shared_ptr<Clock> & clock);

	JsonMethods & getProcedures();
	JsonNotifications & getNotifications();

protected:
	struct ExitProcedure : public JsonRequestProcedure
	{
		ExitProcedure()
		{}

		void call(const Json::Value& request, Json::Value& response) override;
	};

	struct AppApiProcedure : public JsonRequestProcedure
	{
		AppApiProcedure(AppApi & parent) :
			parent(parent)
		{}

		AppApi & parent;
	};

	struct SayHelloProcedure : public AppApiProcedure
	{
		SayHelloProcedure(AppApi & parent) : AppApiProcedure(parent)
		{}

		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	struct GetTasksProcedure : public AppApiProcedure
	{
		GetTasksProcedure(AppApi & parent) : AppApiProcedure(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
		
		void fillJsonValueFromTask(Json::Value& row, const Core::Task & task);
		std::string statusToString(Core::Task::Status status);
	};
	
	struct InsertTask : public AppApiProcedure
	{
		InsertTask(AppApi & parent) : AppApiProcedure(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	struct UpdateTask : public AppApiProcedure
	{
		UpdateTask(AppApi & parent) : AppApiProcedure(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	struct RemoveTask : public AppApiProcedure
	{
		RemoveTask(AppApi & parent) : AppApiProcedure(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	struct GetEvents : public AppApiProcedure
	{
		GetEvents(AppApi & parent) : AppApiProcedure(parent)
		{}
		
		void call(const Json::Value& request, Json::Value& response) override;
	};
	
	std::shared_ptr<AppDB> db;
	std::shared_ptr<Clock> clock;
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
