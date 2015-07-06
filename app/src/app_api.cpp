#include "app_api.hpp"

namespace Tracker
{
namespace Application
{
	
AppApi::AppApi(AppDB & db) :
	db(db),
	sayHello(*this),
	getTasks(*this),
	insertTask(*this),
	updateTask(*this),
	removeTask(*this),
	getEvents(*this)
{
	procedurePointers["exit"] = &exitProcedure;
	procedurePointers["sayHello"] = &sayHello;
	procedurePointers["getTasks"] = &getTasks;
	procedurePointers["insertTask"] = &insertTask;
	procedurePointers["updateTask"] = &updateTask;
	procedurePointers["removeTask"] = &removeTask;
	procedurePointers["getEvents"] = &getEvents;
}

JsonMethods & AppApi::getProcedures()
{
	return procedurePointers;
}
JsonNotifications & AppApi::getNotifications()
{
	return notPointers;
}

void AppApi::ExitProcedure::call(const Json::Value& request, Json::Value& response)
{
	exit(0);
}

void AppApi::SayHelloProcedure::call(const Json::Value& request, Json::Value& response)
{
	response = "Hello: " + request["name"].asString();
}
	
void AppApi::GetTasksProcedure::call(const Json::Value& request, Json::Value& response)
{
	auto result = parent.db.getTasks();
	
	unsigned int i = 0;

	response = Json::Value(Json::arrayValue);
	
	for(auto outer_iter=result->begin(); outer_iter!=result->end(); ++outer_iter) {
		
		auto task = outer_iter->second;
		auto & row = response[i];
		
		fillJsonValueFromTask(row,*task);
		
		i++;

		unsigned int recurringTaskCount = task->getRecurringTaskCount();

		for(unsigned int j = 0; j < recurringTaskCount; j++)
		{
			auto & row = response[i];

			auto child = task->getRecurringChild(j);
			fillJsonValueFromTask(row,*child);

			i++;
		}
	}
}

void AppApi::GetTasksProcedure::fillJsonValueFromTask(Json::Value& row, const Core::Task & task)
{
	row["taskId"] = task.getTaskId();
	row["name"] = task.getName();
	row["earliestStartTime"] = task.getEarliestStartTime();
	row["latestEndTime"] = task.getLatestEndTime();
	row["duration"] = task.getDuration();

	auto recurringParentPtr = task.getRecurranceParent();
	if(recurringParentPtr.expired())
	{
		row["recurringParentTaskId"] = 0;
	}
	else
	{
		auto recurringParent = std::shared_ptr<Core::Task>(recurringParentPtr);
		row["recurringParentTaskId"] = recurringParent->getTaskId();
	}

}
	
void AppApi::InsertTask::call(const Json::Value& request, Json::Value& response)
{
	Core::Task newTask(
			request["name"].asString(),
			request["earliestStartTime"].asInt(),
			request["latestEndTime"].asInt(),
			request["duration"].asInt());
	parent.db.insertTask(newTask);

	response = true;
}
	
void AppApi::UpdateTask::call(const Json::Value& request, Json::Value& response)
{
	Core::Task updatedTask(
			request["name"].asString(),
			request["earliestStartTime"].asInt(),
			request["latestEndTime"].asInt(),
			request["duration"].asInt());
	
	parent.db.updateTask(request["taskId"].asInt(),updatedTask);

	response = true;
}
	
void AppApi::RemoveTask::call(const Json::Value& request, Json::Value& response)
{
	parent.db.removeTask(request["taskId"].asInt());

	response = true;
}
	
void AppApi::GetEvents::call(const Json::Value& request, Json::Value& response)
{
	auto taskList = std::make_shared<std::vector<std::shared_ptr<Core::Task>>>();
	
	auto result = parent.db.getTasks();
	
	for(auto outer_iter=result->begin(); outer_iter!=result->end(); ++outer_iter) {
		
		auto task = outer_iter->second;
		
		taskList->push_back(task);
	}
	
	parent.scheduler.setTaskList(taskList);
	parent.scheduler.schedule();
	
	unsigned int eventCount = parent.scheduler.getScheduledEventCount();
	
	response = Json::Value(Json::arrayValue);

	for(unsigned int i = 0; i < eventCount; i++)
	{
		auto & row = response[i];
		auto event = parent.scheduler.getScheduledEvent(i);
		row["taskId"] = event->getParent()->getTaskId();
		row["name"] = event->getParent()->getName();
		row["startTime"] = event->getStartTime();
		row["duration"] = event->getDuration();
	}

}

	
}
}
