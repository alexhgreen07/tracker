#include "app_api.hpp"

#include <sstream>

using std::make_shared;
using std::to_string;
using std::istringstream;

namespace Tracker
{
namespace Application
{
	
AppApi::AppApi(const shared_ptr<AppDB> & db, const shared_ptr<Clock> & clock) :
	db(db),
	clock(clock),
	exitProcedure(*this),
	resetProcedure(*this),
	sayHello(*this),
	insertTask(*this),
	updateTask(*this),
	updateRecurringTaskStatus(*this),
	removeTask(*this),
	insertEvent(*this),
	updateEvent(*this),
	removeEvent(*this),
	getAppData(*this)
{
	procedurePointers["exit"] = &exitProcedure;
	procedurePointers["reset"] = &resetProcedure;
	procedurePointers["sayHello"] = &sayHello;
	procedurePointers["insertTask"] = &insertTask;
	procedurePointers["updateTask"] = &updateTask;
	procedurePointers["updateRecurringTaskStatus"] = &updateRecurringTaskStatus;
	procedurePointers["removeTask"] = &removeTask;
	procedurePointers["insertEvent"] = &insertEvent;
	procedurePointers["updateEvent"] = &updateEvent;
	procedurePointers["removeEvent"] = &removeEvent;

	procedurePointers["getAppData"] = &getAppData;
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
	string connectionString = baseDb->getConnectionString();
	baseDb->close();
	baseDb->open(connectionString);
	parent.db->updateDatabase();

	response = true;
}

void AppApi::SayHelloProcedure::call(const Json::Value& request, Json::Value& response)
{
	response = "Hello: " + request["name"].asString();
}

void AppApi::fillJsonValueFromTask(Json::Value& row, const Core::Task & task)
{
	row["taskId"] = task.getTaskId();
	row["name"] = task.getName();
	row["earliestStartTime"] = to_string(task.getEarliestStartTime());
	row["latestEndTime"] = to_string(task.getLatestEndTime());
	row["duration"] = to_string(task.getDuration());
	row["status"] = taskStatusToString(task.getStatus());
	row["recurringPeriod"] = to_string(task.getRecurringPeriod());
	row["recurringLateOffset"] = to_string(task.getRecurringLateOffset());
	row["recurringCount"] = static_cast<unsigned int>(task.getRecurringTaskCount());
}

string AppApi::taskStatusToString(Core::Task::Status status)
{
	string statusString;

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

Core::Task::Status AppApi::taskStatusFromString(string status)
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
	row["eventId"] = to_string(event.getEventId());

	string taskId = to_string(event.getParent()->getTaskId());
	if(!event.getParent()->getRecurranceParent().expired())
	{
		auto recurringParent = event.getParent()->getRecurranceParent().lock();
		taskId = to_string(recurringParent->getTaskId());
	}
	row["taskId"] = taskId;

	row["startTime"] = to_string(event.getStartTime());
	row["duration"] = to_string(event.getDuration());
	row["status"] = eventStatusToString(event.getStatus());
	row["recurringIndex"] = event.getParent()->getRecurringIndex();

	row["taskStatus"] = taskStatusToString(event.getParent()->getStatus());

	row["isLate"] = false;
	if((event.getStatus() == Event::Status::Scheduled) &&
			(event.getEndTime() > event.getParent()->getLatestEndTime()))
	{
		row["isLate"] = true;
	}
}

string AppApi::eventStatusToString(Core::Event::Status status)
{
	string statusString;

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

Core::Event::Status AppApi::eventStatusFromString(string status)
{
	Core::Event::Status returnValue;

	if(status == "Scheduled")
	{
		returnValue = Core::Event::Status::Scheduled;
	}
	else if(status == "Running")
	{
		returnValue = Core::Event::Status::Running;
	}
	else
	{
		returnValue = Core::Event::Status::Logged;
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

	istringstream(request["earliestStartTime"].asString()) >> earliestStartTime;
	istringstream(request["latestEndTime"].asString()) >> latestEndTime;
	istringstream(request["duration"].asString()) >> duration;
	status = taskStatusFromString(request["status"].asString());
	istringstream(request["recurringPeriod"].asString()) >> recurringPeriod;
	istringstream(request["recurringLateOffset"].asString()) >> recurringLateOffset;

	auto newTask = make_shared<Core::Task>(
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

	istringstream(request["earliestStartTime"].asString()) >> earliestStartTime;
	istringstream(request["latestEndTime"].asString()) >> latestEndTime;
	istringstream(request["duration"].asString()) >> duration;
	status = taskStatusFromString(request["status"].asString());
	istringstream(request["recurringPeriod"].asString()) >> recurringPeriod;
	istringstream(request["recurringLateOffset"].asString()) >> recurringLateOffset;

	auto updatedTask = make_shared<Core::Task>(
				request["name"].asString(),
				earliestStartTime,
				latestEndTime,
				duration);
	updatedTask->setStatus(status);
	updatedTask->setRecurranceParameters(recurringPeriod,recurringLateOffset);
	
	parent.db->updateTask(request["taskId"].asInt(),*updatedTask);

	response = true;
}

void AppApi::UpdateRecurringTaskStatus::call(const Json::Value& request, Json::Value& response)
{
	uint64_t taskId;
	uint64_t recurringIndex;
	Task::Status status;

	istringstream(request["taskId"].asString()) >> taskId;
	istringstream(request["recurringIndex"].asString()) >> recurringIndex;
	status = taskStatusFromString(request["status"].asString());

	parent.db->updateRecurringTaskStatus(taskId,recurringIndex,(Task::Status)status);

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

	istringstream(request["startTime"].asString()) >> startTime;
	istringstream(request["duration"].asString()) >> duration;
	istringstream(request["taskId"].asString()) >> parentTaskId;
	istringstream(request["recurringIndex"].asString()) >> recurringIndex;
	status = eventStatusFromString(request["status"].asString());

	auto result = parent.db->getAppData()->tasks;
	auto parentTaskAtId = result->at(parentTaskId);

	shared_ptr<const Core::Task> parentTask = parentTaskAtId;

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
	Core::Event::Status status;

	istringstream(request["eventId"].asString()) >> eventId;
	istringstream(request["startTime"].asString()) >> startTime;
	istringstream(request["duration"].asString()) >> duration;
	istringstream(request["taskId"].asString()) >> parentTaskId;
	istringstream(request["recurringIndex"].asString()) >> recurringIndex;
	status = eventStatusFromString(request["status"].asString());

	auto result = parent.db->getAppData()->tasks;
	auto parentTaskAtId = result->at(parentTaskId);

	shared_ptr<const Core::Task> parentTask = parentTaskAtId;

	if(parentTaskAtId->getIsRecurringParent())
	{
		parentTask = parentTaskAtId->getRecurringChild(recurringIndex);
	}

	Core::Event updatedEvent(startTime,duration);
	updatedEvent.setStatus(status);
	updatedEvent.setParent(parentTask);

	parent.db->updateEvent(eventId,updatedEvent);

	response = true;
}

void AppApi::RemoveEvent::call(const Json::Value& request, Json::Value& response)
{
	uint64_t eventId;

	istringstream input_stream(request["eventId"].asString());
	input_stream >> eventId;

	parent.db->removeEvent(eventId);

	response = true;
}

void AppApi::fillTaskData(shared_ptr<AppDB::AppData> data, Json::Value& response)
{
	unsigned int rowCount = 0;
	auto result = data->tasks;

	rowCount = 0;

	response["tasks"] = Json::Value(Json::arrayValue);

	for(auto outer_iter=result->begin(); outer_iter!=result->end(); ++outer_iter) {

		auto task = outer_iter->second;
		auto & row = response["tasks"][rowCount];

		fillJsonValueFromTask(row,*task);

		rowCount++;
	}
}

void AppApi::fillEventData(shared_ptr<AppDB::AppData> data, Json::Value& response,
		uint64_t startTime, uint64_t endTime)
{
	unsigned int rowCount = 0;

	//build the events array
	response["events"] = Json::Value(Json::arrayValue);

	auto taskList = make_shared<vector<shared_ptr<Core::Task>>>();
	
	auto loggedEvents = data->loggedEvents;
	auto loggedEventsList = make_shared<vector<shared_ptr<Core::Event>>>();

	rowCount = 0;

	for(auto iter = loggedEvents->begin(); iter != loggedEvents->end(); ++iter)
	{
		auto event = iter->second;
		loggedEventsList->push_back(event);

		if(event->overlaps(startTime,endTime))
		{
			auto & row = response["events"][rowCount];

			//ensure we set the duration for running tasks
			if(event->getStatus() == Core::Event::Status::Running)
			{
				if(event->getStartTime() < clock->getNowTimestamp())
				{
					uint64_t runningDuration = clock->getNowTimestamp() - event->getStartTime();
					event->setDuration(runningDuration);
				}
			}

			fillJsonValueFromEvent(row,*event);

			rowCount++;
		}
	}

	for(auto iter = data->tasks->begin(); iter != data->tasks->end(); ++iter) {
		
		auto task = iter->second;
		
		taskList->push_back(task);
	}
	
	scheduler.setLoggedEventList(loggedEventsList);
	scheduler.setTaskList(taskList);
	scheduler.schedule(clock->getNowTimestamp());
	
	unsigned int eventCount = scheduler.getScheduledEventCount();

	for(unsigned int i = 0; i < eventCount; i++)
	{
		auto event = scheduler.getScheduledEvent(i);

		if(event->overlaps(startTime,endTime))
		{
			auto & row = response["events"][rowCount];

			fillJsonValueFromEvent(row,*event);

			rowCount++;
		}

	}
}

void AppApi::GetAppData::call(const Json::Value& request, Json::Value& response)
{
	uint64_t startTime = 0;
	uint64_t endTime = numeric_limits<uint64_t>::max();
	if(request.isMember("startTime") && request.isMember("endTime"))
	{
		istringstream(request["startTime"].asString()) >> startTime;
		istringstream(request["endTime"].asString()) >> endTime;
	}

	auto data = parent.db->getAppData();

	parent.fillTaskData(data,response);
	parent.fillEventData(data,response,startTime,endTime);
}

	
}
}
