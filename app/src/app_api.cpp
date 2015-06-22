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
	updateTask(*this)
{
	procedurePointers["sayHello"] = &sayHello;
	procedurePointers["getTasks"] = &getTasks;
	procedurePointers["insertTask"] = &insertTask;
	procedurePointers["updateTask"] = &updateTask;
}

JsonMethods & AppApi::getProcedures()
{
	return procedurePointers;
}
JsonNotifications & AppApi::getNotifications()
{
	return notPointers;
}
	
void AppApi::SayHelloProcedure::call(const Json::Value& request, Json::Value& response)
{
	response = "Hello: " + request["name"].asString();
}
	
void AppApi::GetTasksProcedure::call(const Json::Value& request, Json::Value& response)
{
	auto result = parent.db.getTasks();
	
	unsigned int i = 1;
	
	for(auto outer_iter=result->begin(); outer_iter!=result->end(); ++outer_iter) {
		
		auto task = outer_iter->second;
		auto & row = response[i];
		
		row["taskId"] = (unsigned int)outer_iter->first;
		row["earliestStartTime"] = task.getEarliestStartTime();
		row["latestEndTime"] = task.getLatestEndTime();
		row["duration"] = task.getDuration();
		
		i++;
	}
}
	
void AppApi::InsertTask::call(const Json::Value& request, Json::Value& response)
{
	Core::Task newTask(
	   request["earliestStartTime"].asInt(),
	   request["latestEndTime"].asInt(),
	   request["duration"].asInt());
	parent.db.insertTask(newTask);
}
	
void AppApi::UpdateTask::call(const Json::Value& request, Json::Value& response)
{
	Core::Task updatedTask(
	   request["earliestStartTime"].asInt(),
	   request["latestEndTime"].asInt(),
	   request["duration"].asInt());
	
	parent.db.updateTask(request["taskId"].asInt(),updatedTask);
}

	
}
}