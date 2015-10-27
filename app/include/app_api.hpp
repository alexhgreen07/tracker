#include <api.hpp>

#include "app_db.hpp"
#include <scheduler.hpp>

using std::shared_ptr;
using std::string;

namespace Tracker
{
using namespace Core;

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

	AppApi(const shared_ptr<AppDB> & db, const shared_ptr<Clock> & clock);

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

	void fillJsonValueFromTask(Json::Value& row, const Task & task);
	string taskStatusToString(Task::Status status);
	static Task::Status taskStatusFromString(string status);
	void fillJsonValueFromEvent(Json::Value& row, const Event & event);
	string eventStatusToString(Event::Status status);
	static Event::Status eventStatusFromString(string status);

	shared_ptr<AppDB> db;
	shared_ptr<Clock> clock;
	Scheduler scheduler;

	APP_API_PROCEDURE(ExitProcedure) exitProcedure;
	APP_API_PROCEDURE(ResetProcedure) resetProcedure;
	APP_API_PROCEDURE(SayHelloProcedure) sayHello;

	APP_API_PROCEDURE(InsertTask) insertTask;
	APP_API_PROCEDURE(UpdateTask) updateTask;
	APP_API_PROCEDURE(UpdateRecurringTaskStatus) updateRecurringTaskStatus;
	APP_API_PROCEDURE(RemoveTask) removeTask;
	APP_API_PROCEDURE(InsertEvent) insertEvent;
	APP_API_PROCEDURE(UpdateEvent) updateEvent;
	APP_API_PROCEDURE(RemoveEvent) removeEvent;
	
	APP_API_PROCEDURE(GetAppData) getAppData;

	JsonMethods procedurePointers;
	JsonNotifications notPointers;
};

}
}
