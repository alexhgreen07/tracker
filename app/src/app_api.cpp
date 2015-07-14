#include "app_api.hpp"

#include <sstream>

namespace Tracker
{
namespace Application
{
	
AppApi::AppApi(const std::shared_ptr<AppDB> & db, const std::shared_ptr<Clock> & clock) :
	db(db),
	clock(clock),
	exitProcedure(*this),
	sayHello(*this),
	getTasks(*this),
	insertTask(*this),
	updateTask(*this),
	removeTask(*this),
	insertEvent(*this),
	updateEvent(*this),
	getEvents(*this)
{
	procedurePointers["exit"] = &exitProcedure;
	procedurePointers["sayHello"] = &sayHello;
	procedurePointers["getTasks"] = &getTasks;
	procedurePointers["insertTask"] = &insertTask;
	procedurePointers["updateTask"] = &updateTask;
	procedurePointers["removeTask"] = &removeTask;
	procedurePointers["insertEvent"] = &insertEvent;
	procedurePointers["updateEvent"] = &updateEvent;
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
	auto result = parent.db->getTasks();
	
	unsigned int i = 0;

	response = Json::Value(Json::arrayValue);
	
	for(auto outer_iter=result->begin(); outer_iter!=result->end(); ++outer_iter) {
		
		auto task = outer_iter->second;
		auto & row = response[i];
		
		parent.fillJsonValueFromTask(row,*task);
		
		i++;
	}
}

void AppApi::fillJsonValueFromTask(Json::Value& row, const Core::Task & task)
{
	row["taskId"] = task.getTaskId();
	row["name"] = task.getName();
	row["earliestStartTime"] = std::to_string(task.getEarliestStartTime());
	row["latestEndTime"] = std::to_string(task.getLatestEndTime());
	row["duration"] = std::to_string(task.getDuration());
	row["status"] = statusToString(task.getStatus());
	row["recurringPeriod"] = std::to_string(task.getRecurringPeriod());
	row["recurringLateOffset"] = std::to_string(task.getRecurringLateOffset());

}

std::string AppApi::statusToString(Core::Task::Status status)
{
	std::string statusString;

	switch(status)
	{
	case Core::Task::Status::Incomplete:
		statusString = "Incomplete";
		break;
	case Core::Task::Status::Complete:
		statusString = "Complete";
		break;
	case Core::Task::Status::Missed:
		statusString = "Missed";
		break;
	default:
		statusString = "";
		break;
	}

	return statusString;
}

Core::Task::Status AppApi::taskStatusFromString(std::string status)
{
	Core::Task::Status returnStatus;

	if(status == "Complete")
	{
		returnStatus = Core::Task::Status::Complete;
	}
	else if(status == "Missed")
	{
		returnStatus = Core::Task::Status::Missed;
	}
	else
	{
		returnStatus = Core::Task::Status::Incomplete;
	}
	
	return returnStatus;
}

void AppApi::InsertTask::call(const Json::Value& request, Json::Value& response)
{
	uint64_t earliestStartTime;
	uint64_t latestEndTime;
	uint64_t duration;
	uint64_t recurringPeriod;
	uint64_t recurringLateOffset;
	Core::Task::Status status;

	std::istringstream input_stream(request["earliestStartTime"].asString());
	input_stream >> earliestStartTime;

	input_stream = std::istringstream(request["latestEndTime"].asString());
	input_stream >> latestEndTime;

	input_stream = std::istringstream(request["duration"].asString());
	input_stream >> duration;

	status = taskStatusFromString(request["status"].asString());

	input_stream = std::istringstream(request["recurringPeriod"].asString());
	input_stream >> recurringPeriod;

	input_stream = std::istringstream(request["recurringLateOffset"].asString());
	input_stream >> recurringLateOffset;

	auto newTask = std::make_shared<Core::Task>(
			request["name"].asString(),
			earliestStartTime,
			latestEndTime,
			duration);
	newTask->setStatus(status);
	newTask->setRecurranceParameters(recurringPeriod,recurringLateOffset);
	parent.db->insertTask(*newTask);

	response = true;
}
	
void AppApi::UpdateTask::call(const Json::Value& request, Json::Value& response)
{
	uint64_t earliestStartTime;
	uint64_t latestEndTime;
	uint64_t duration;
	uint64_t recurringPeriod;
	uint64_t recurringLateOffset;
	Core::Task::Status status;

	std::istringstream input_stream(request["earliestStartTime"].asString());
	input_stream >> earliestStartTime;

	input_stream = std::istringstream(request["latestEndTime"].asString());
	input_stream >> latestEndTime;

	input_stream = std::istringstream(request["duration"].asString());
	input_stream >> duration;

	status = taskStatusFromString(request["status"].asString());

	input_stream = std::istringstream(request["recurringPeriod"].asString());
	input_stream >> recurringPeriod;

	input_stream = std::istringstream(request["recurringLateOffset"].asString());
	input_stream >> recurringLateOffset;

	auto updatedTask = std::make_shared<Core::Task>(
				request["name"].asString(),
				earliestStartTime,
				latestEndTime,
				duration);
	updatedTask->setStatus(status);
	updatedTask->setRecurranceParameters(recurringPeriod,recurringLateOffset);
	
	parent.db->updateTask(request["taskId"].asInt(),*updatedTask);

	response = true;
}
	
void AppApi::RemoveTask::call(const Json::Value& request, Json::Value& response)
{
	parent.db->removeTask(request["taskId"].asInt());

	response = true;
}

void AppApi::InsertEvent::call(const Json::Value& request, Json::Value& response)
{
	uint64_t startTime;
	uint64_t duration;
	uint64_t parentTaskId;

	std::istringstream input_stream(request["startTime"].asString());
	input_stream >> startTime;

	input_stream = std::istringstream(request["duration"].asString());
	input_stream >> duration;

	input_stream = std::istringstream(request["taskId"].asString());
	input_stream >> parentTaskId;

	auto result = parent.db->getTasks();
	auto parentTask = result->at(parentTaskId);

	Core::Event newEvent(startTime,duration);
	newEvent.setParent(parentTask);

	parent.db->insertEvent(newEvent);

	response = true;
}

void AppApi::UpdateEvent::call(const Json::Value& request, Json::Value& response)
{
	uint64_t eventId;
	uint64_t startTime;
	uint64_t duration;
	uint64_t parentTaskId;

	std::istringstream input_stream(request["eventId"].asString());
	input_stream >> eventId;

	input_stream = std::istringstream(request["startTime"].asString());
	input_stream >> startTime;

	input_stream = std::istringstream(request["duration"].asString());
	input_stream >> duration;

	input_stream = std::istringstream(request["taskId"].asString());
	input_stream >> parentTaskId;

	auto result = parent.db->getTasks();
	auto parentTask = result->at(parentTaskId);

	Core::Event updatedEvent(startTime,duration);
	updatedEvent.setParent(parentTask);

	parent.db->updateEvent(eventId,updatedEvent);

	response = true;
}

void AppApi::GetEvents::call(const Json::Value& request, Json::Value& response)
{
	auto taskList = std::make_shared<std::vector<std::shared_ptr<Core::Task>>>();
	
	auto result = parent.db->getTasks();
	
	for(auto outer_iter=result->begin(); outer_iter!=result->end(); ++outer_iter) {
		
		auto task = outer_iter->second;
		
		taskList->push_back(task);
	}
	
	parent.scheduler.setTaskList(taskList);
	parent.scheduler.schedule(parent.clock->getNowTimestamp());
	
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
