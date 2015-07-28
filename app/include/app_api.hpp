#include <api.hpp>

#include "app_db.hpp"
#include <scheduler.hpp>

namespace Tracker
{
namespace Application
{
using namespace Network;

#define APP_API_PROCEDURE(eventName) \
struct eventName : public AppApiProcedure { \
	eventName(AppApi & parent) : AppApiProcedure(parent) {} \
	void call(const Json::Value& request, Json::Value& response) override; \
}

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

	struct AppApiProcedure : public JsonRequestProcedure
	{
		AppApiProcedure(AppApi & parent) :
			parent(parent)
		{}

		AppApi & parent;
	};

	void fillJsonValueFromTask(Json::Value& row, const Core::Task & task);
	std::string taskStatusToString(Core::Task::Status status);
	static Core::Task::Status taskStatusFromString(std::string status);
	void fillJsonValueFromEvent(Json::Value& row, const Core::Event & event);
	std::string eventStatusToString(Core::Event::Status status);

	std::shared_ptr<AppDB> db;
	std::shared_ptr<Clock> clock;
	Core::Scheduler scheduler;

	APP_API_PROCEDURE(ExitProcedure) exitProcedure;
	APP_API_PROCEDURE(ResetProcedure) resetProcedure;
	APP_API_PROCEDURE(SayHelloProcedure) sayHello;

	APP_API_PROCEDURE(GetTasksProcedure) getTasks;
	APP_API_PROCEDURE(InsertTask) insertTask;
	APP_API_PROCEDURE(UpdateTask) updateTask;
	APP_API_PROCEDURE(RemoveTask) removeTask;
	APP_API_PROCEDURE(InsertEvent) insertEvent;
	APP_API_PROCEDURE(UpdateEvent) updateEvent;
	APP_API_PROCEDURE(RemoveEvent) removeEvent;
	APP_API_PROCEDURE(GetEvents) getEvents;
	
	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
