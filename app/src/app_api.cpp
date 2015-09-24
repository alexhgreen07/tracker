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
	resetProcedure(*this),
	sayHello(*this),
	getTasks(*this),
	insertTask(*this),
	updateTask(*this),
	removeTask(*this),
	insertEvent(*this),
	updateEvent(*this),
	removeEvent(*this),
	getEvents(*this)
{
	procedurePointers["exit"] = &exitProcedure;
	procedurePointers["reset"] = &resetProcedure;
	procedurePointers["sayHello"] = &sayHello;
	procedurePointers["getTasks"] = &getTasks;
	procedurePointers["insertTask"] = &insertTask;
	procedurePointers["updateTask"] = &updateTask;
	procedurePointers["removeTask"] = &removeTask;
	procedurePointers["insertEvent"] = &insertEvent;
	procedurePointers["updateEvent"] = &updateEvent;
	procedurePointers["removeEvent"] = &removeEvent;
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

void AppApi::ResetProcedure::call(const Json::Value& request, Json::Value& response)
{
	//NOTE: this is for debug purposes only to support web testing
	auto baseDb = parent.db->getDatabase();
	std::string connectionString = baseDb->getConnectionString();
	baseDb->close();
	baseDb->open(connectionString);
	parent.db->updateDatabase();

	response = true;
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
	row["status"] = taskStatusToString(task.getStatus());
	row["recurringPeriod"] = std::to_string(task.getRecurringPeriod());
	row["recurringLateOffset"] = std::to_string(task.getRecurringLateOffset());
	row["recurringCount"] = static_cast<unsigned int>(task.getRecurringTaskCount());
}

std::string AppApi::taskStatusToString(Core::Task::Status status)
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

void AppApi::fillJsonValueFromEvent(Json::Value& row, const Core::Event & event)
{
	row["eventId"] = std::to_string(event.getEventId());

	std::string taskId = std::to_string(event.getParent()->getTaskId());
	if(!event.getParent()->getRecurranceParent().expired())
	{
		auto recurringParent = event.getParent()->getRecurranceParent().lock();
		taskId = std::to_string(recurringParent->getTaskId());
	}
	row["taskId"] = taskId;

	row["startTime"] = std::to_string(event.getStartTime());
	row["duration"] = std::to_string(event.getDuration());
	row["status"] = eventStatusToString(event.getStatus());
	row["recurringIndex"] = event.getParent()->getRecurringIndex();
}

std::string AppApi::eventStatusToString(Core::Event::Status status)
{
	std::string statusString;

	switch(status)
	{
	case Core::Event::Status::Logged:
		statusString = "Logged";
		break;
	case Core::Event::Status::Running:
		statusString = "Running";
		break;
	case Core::Event::Status::Scheduled:
		statusString = "Scheduled";
		break;
	default:
		statusString = "";
		break;
	}

	return statusString;
}

Core::Event::Status AppApi::eventStatusFromString(std::string status)
{
	Core::Event::Status returnValue;

	if(status == "Logged")
	{
		returnValue = Core::Event::Status::Logged;
	}
	else if(status == "Running")
	{
		returnValue = Core::Event::Status::Running;
	}
	else
	{
		returnValue = Core::Event::Status::Scheduled;
	}

	return returnValue;
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
	uint64_t recurringIndex;
	Core::Event::Status status;

	std::istringstream input_stream(request["startTime"].asString());
	input_stream >> startTime;

	input_stream = std::istringstream(request["duration"].asString());
	input_stream >> duration;

	input_stream = std::istringstream(request["taskId"].asString());
	input_stream >> parentTaskId;

	input_stream = std::istringstream(request["recurringIndex"].asString());
	input_stream >> recurringIndex;

	status = eventStatusFromString(request["status"].asString());

	auto result = parent.db->getTasks();
	auto parentTaskAtId = result->at(parentTaskId);

	std::shared_ptr<const Core::Task> parentTask = parentTaskAtId;

	if(parentTaskAtId->getIsRecurringParent())
	{
		parentTask = parentTaskAtId->getRecurringChild(recurringIndex);
	}

	Core::Event newEvent(startTime,duration);
	newEvent.setStatus(status);
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
	uint64_t recurringIndex;

	std::istringstream input_stream(request["eventId"].asString());
	input_stream >> eventId;

	input_stream = std::istringstream(request["startTime"].asString());
	input_stream >> startTime;

	input_stream = std::istringstream(request["duration"].asString());
	input_stream >> duration;

	input_stream = std::istringstream(request["taskId"].asString());
	input_stream >> parentTaskId;

	input_stream = std::istringstream(request["recurringIndex"].asString());
	input_stream >> recurringIndex;

	auto result = parent.db->getTasks();
	auto parentTaskAtId = result->at(parentTaskId);

	std::shared_ptr<const Core::Task> parentTask = parentTaskAtId;

	if(parentTaskAtId->getIsRecurringParent())
	{
		parentTask = parentTaskAtId->getRecurringChild(recurringIndex);
	}

	Core::Event updatedEvent(startTime,duration);
	updatedEvent.setParent(parentTask);

	parent.db->updateEvent(eventId,updatedEvent);

	response = true;
}

void AppApi::RemoveEvent::call(const Json::Value& request, Json::Value& response)
{
	uint64_t eventId;

	std::istringstream input_stream(request["eventId"].asString());
	input_stream >> eventId;

	parent.db->removeEvent(eventId);

	response = true;
}

void AppApi::GetEvents::call(const Json::Value& request, Json::Value& response)
{
	unsigned int rowCount = 0;
	response = Json::Value(Json::arrayValue);

	auto taskList = std::make_shared<std::vector<std::shared_ptr<Core::Task>>>();
	auto result = parent.db->getTasks();
	
	auto loggedEvents = parent.db->getLoggedEvents();
	auto loggedEventsList = std::make_shared<std::vector<std::shared_ptr<Core::Event>>>();

	for(auto iter = loggedEvents->begin(); iter != loggedEvents->end(); ++iter)
	{
		auto & row = response[rowCount];
		auto event = iter->second;
		parent.fillJsonValueFromEvent(row,*event);

		rowCount++;

		loggedEventsList->push_back(event);
	}

	for(auto iter = result->begin(); iter != result->end(); ++iter) {
		
		auto task = iter->second;
		
		taskList->push_back(task);
	}
	
	parent.scheduler.setLoggedEventList(loggedEventsList);
	parent.scheduler.setTaskList(taskList);
	parent.scheduler.schedule(parent.clock->getNowTimestamp());
	
	unsigned int eventCount = parent.scheduler.getScheduledEventCount();

	for(unsigned int i = 0; i < eventCount; i++)
	{
		auto & row = response[rowCount];
		auto event = parent.scheduler.getScheduledEvent(i);

		parent.fillJsonValueFromEvent(row,*event);

		rowCount++;
	}

}

	
}
}
