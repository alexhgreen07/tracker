#include "app_db.hpp"

#include <sstream>

using std::shared_ptr;
using std::make_shared;

using std::map;
using std::string;

using std::to_string;
using std::istringstream;

namespace Tracker
{
namespace Application
{

AppDB::AppDB(const shared_ptr<Database::Database> & database) :
    database(database),
    currentVersion("0")
{}

shared_ptr<Database::Database> AppDB::getDatabase()
{
	return database;
}

void AppDB::updateDatabase()
{
	bool shouldInitialize = false;

	try
	{
		auto tasksTable = database->select("select version from version");

		if(tasksTable->at(0).at(0) != currentVersion)
		{
			shouldInitialize = true;
		}
	}
	catch(const std::exception& e)
	{
		//failed to query, initialize new database
		shouldInitialize = true;
	}

	if(shouldInitialize)
	{
		initializeNewDatabase();
	}

}

void AppDB::initializeNewDatabase()
{
    database->execute("create table version (version int)");
    
    createVersionTable();
    createTasksTable();
    createRecurringTasksStatusTable();
    createEventsTable();
}

void AppDB::createVersionTable()
{
    database->execute("insert into version values (" + currentVersion + ")");
}

void AppDB::createTasksTable()
{
    string createSql = "create table tasks (";
    createSql += "taskId integer primary key asc";
    createSql += ",name text";
    createSql += ",earliestStartTime integer";
    createSql += ",latestEndTime integer";
    createSql += ",duration integer";
    createSql += ",status integer";
    createSql += ",recurringPeriod integer";
    createSql += ",recurringLateOffset integer";
    createSql += ");";
    
    database->execute(createSql);
}

void AppDB::createRecurringTasksStatusTable()
{
	string createSql = "create table recurring_task_status (";
	createSql += "recurringTaskStatusId integer primary key asc";
	createSql += ",taskId integer";
	createSql += ",recurringIndex integer";
	createSql += ",status integer";
	createSql += ");";

	database->execute(createSql);
}

void AppDB::createEventsTable()
{
	string createSql = "create table events (";
	createSql += "eventId integer primary key asc";
	createSql += ",startTime integer";
	createSql += ",duration integer";
	createSql += ",taskId integer";
	createSql += ",status integer";
	createSql += ",recurringIndex integer";
	createSql += ");";

	database->execute(createSql);
}

shared_ptr<map<uint64_t, shared_ptr<Task>>> AppDB::getTasks()
{
    auto tasksTable = database->select("select taskId, name, earliestStartTime, latestEndTime, duration, status, recurringPeriod, recurringLateOffset from tasks");
    tasks = make_shared<map<uint64_t, shared_ptr<Task>>>();
    
    auto recurringTasksStatusTable =
			database->select("select taskId, recurringIndex, status from recurring_task_status");
	auto recurringTaskStatusMap = map<uint64_t,map<uint64_t,Task::Status>>();
	for(unsigned int i = 0; i < recurringTasksStatusTable->size(); i++)
	{
		uint64_t taskId;
		uint64_t recurringIndex;
		uint64_t status;
		auto row = recurringTasksStatusTable->at(i);

		istringstream input_stream(row[0]);
		input_stream >> taskId;
		input_stream = istringstream(row[1]);
		input_stream >> recurringIndex;
		input_stream = istringstream(row[2]);
		input_stream >> status;

		recurringTaskStatusMap[taskId][recurringIndex] = (Task::Status)status;
	}

    for(unsigned int i = 0; i < tasksTable->size(); i++)
    {
        auto nextTask = make_shared<Task>();
        auto row = tasksTable->at(i);

        uint64_t value;
        uint64_t taskId;

        istringstream input_stream(row[0]);
        input_stream >> taskId;
        nextTask->setTaskId(taskId);

        nextTask->setName(row[1]);
        input_stream = istringstream(row[2]);
        input_stream >> value;
        nextTask->setEarliestStartTime(value);

        input_stream = istringstream(row[3]);
        input_stream >> value;
        nextTask->setLatestEndTime(value);

        input_stream = istringstream(row[4]);
		input_stream >> value;
        nextTask->setDuration(value);

        input_stream = istringstream(row[5]);
		input_stream >> value;
		nextTask->setStatus((Task::Status)value);

        uint64_t recurrancePeriod;
        uint64_t recurranceLateOffset;
        input_stream = istringstream(row[6]);
		input_stream >> recurrancePeriod;
		input_stream = istringstream(row[7]);
		input_stream >> recurranceLateOffset;
        nextTask->setRecurranceParameters(recurrancePeriod,recurranceLateOffset);

        auto recurringStatusEntry = recurringTaskStatusMap[taskId];
        for(unsigned int j = 0; j < nextTask->getRecurringTaskCount(); j++)
        {
        	nextTask->setRecurringChildStatus(j,recurringStatusEntry[j]);
        }
		
		(*tasks)[taskId] = nextTask;
    }

    return tasks;
}

uint64_t AppDB::insertTask(const Task & newTask)
{
	uint64_t taskInsertedRowId = 0;

    string columnsString = "";
    string valuesString = "";
	
	columnsString += "name";
	valuesString += "'" + newTask.getName() + "'";
    columnsString += ",earliestStartTime";
    valuesString += "," + to_string(newTask.getEarliestStartTime());
    columnsString += ",latestEndTime";
    valuesString += "," + to_string(newTask.getLatestEndTime());
    columnsString += ",duration";
    valuesString += "," + to_string(newTask.getDuration());
	columnsString += ",status";
    valuesString += "," + to_string((uint64_t)newTask.getStatus());
    columnsString += ",recurringPeriod";
	valuesString += "," + to_string(newTask.getRecurringPeriod());
	columnsString += ",recurringLateOffset";
	valuesString += "," + to_string(newTask.getRecurringLateOffset());
    
    string insertString =
    "insert into tasks (" +
    columnsString + ") values(" + valuesString + ")";
    
    database->execute(insertString);
    taskInsertedRowId = database->lastInsertRowId();

    for(unsigned int index = 0; index < newTask.getRecurringTaskCount(); index++)
    {
    	auto recurringChild = newTask.getRecurringChild(index);
    	insertRecurringTaskStatus(taskInsertedRowId,index,recurringChild->getStatus());
    }

    getTasks();

    return taskInsertedRowId;
}

void AppDB::updateTask(uint64_t taskId, Task & task)
{
    string updateString = "update tasks set ";
    
    updateString += "name = '" + task.getName() + "'";
    updateString += ",earliestStartTime = " + to_string(task.getEarliestStartTime());
    updateString += ",latestEndTime = " + to_string(task.getLatestEndTime());
    updateString += ",duration = " + to_string(task.getDuration());
    updateString += ",status = " + to_string((uint64_t)task.getStatus());
    updateString += ",recurringPeriod = " + to_string(task.getRecurringPeriod());
    updateString += ",recurringLateOffset = " + to_string(task.getRecurringLateOffset());
    
    updateString += " where taskId = " + to_string(taskId);
    
    database->execute(updateString);

    getTasks();
}

void AppDB::removeTask(uint64_t taskId)
{
    string deleteString =
    "delete from tasks where taskId = " + to_string(taskId);
    
    database->execute(deleteString);

    removeRecurringTaskStatus(taskId);

    getTasks();
}

uint64_t AppDB::insertRecurringTaskStatus(uint64_t taskId, uint64_t recurringIndex, Task::Status status)
{
	string columnsString = "";
	string valuesString = "";

	columnsString += "taskId";
	valuesString += "'" + to_string(taskId) + "'";
	columnsString += ",recurringIndex";
	valuesString += "," + to_string(recurringIndex);
	columnsString += ",status";
	valuesString += "," + to_string((unsigned int)status);

	string insertString =
			"insert into recurring_task_status (" +
			columnsString + ") values(" + valuesString + ")";

	database->execute(insertString);
	return database->lastInsertRowId();
}

void AppDB::updateRecurringTaskStatus(uint64_t taskId, uint64_t recurringIndex, Task::Status status)
{
	string updateString = "update recurring_task_status set ";

	updateString += "status = " + to_string((unsigned int)status);

	updateString += " where (taskId = " + to_string(taskId) +
			") and (recurringIndex = " + to_string(recurringIndex) + ")";

	database->execute(updateString);
}

void AppDB::removeRecurringTaskStatus(uint64_t taskId)
{
	string deleteString =
			"delete from recurring_task_status where taskId = " + to_string(taskId);
	database->execute(deleteString);
}

shared_ptr<map<uint64_t, shared_ptr<Event>>> AppDB::getLoggedEvents()
{
	auto eventsTable = database->select("select eventId, startTime, duration, taskId, recurringIndex, status from events");
	events = make_shared<map<uint64_t, shared_ptr<Event>>>();

	if(!tasks)
	{
		getTasks();
	}

	for(unsigned int i = 0; i < eventsTable->size(); i++)
	{
		auto nextEvent = make_shared<Event>();
		auto row = eventsTable->at(i);

		uint64_t value;
		uint64_t eventId;
		uint64_t recurringIndex;

		istringstream input_stream(row[0]);
		input_stream >> eventId;
		nextEvent->setEventId(eventId);

		input_stream = istringstream(row[1]);
		input_stream >> value;
		nextEvent->setStartTime(value);

		input_stream = istringstream(row[2]);
		input_stream >> value;
		nextEvent->setDuration(value);

		input_stream = istringstream(row[3]);
		input_stream >> value;
		
		auto parentTaskAtId = (*tasks)[value];

		input_stream = istringstream(row[4]);
		input_stream >> recurringIndex;

		input_stream = istringstream(row[5]);
		input_stream >> value;
		nextEvent->setStatus((Event::Status)value);

		if(parentTaskAtId->getRecurringTaskCount() > 0)
		{
			nextEvent->setParent(parentTaskAtId->getRecurringChild(recurringIndex));
		}
		else
		{
			nextEvent->setParent(parentTaskAtId);
		}

		(*events)[eventId] = nextEvent;
	}

	return events;
}

uint64_t AppDB::insertEvent(const Event & newEvent)
{
	uint64_t eventInsertedRowId = 0;

	string columnsString = "";
	string valuesString = "";

	columnsString += "startTime";
	valuesString += to_string(newEvent.getStartTime());
	columnsString += ",duration";
	valuesString += "," + to_string(newEvent.getDuration());

	uint64_t parentTaskId;
	auto parent = newEvent.getParent();
	parentTaskId = parent->getTaskId();
	if(!parent->getRecurranceParent().expired())
	{
		auto recurringParent = parent->getRecurranceParent().lock();
		parentTaskId = recurringParent->getTaskId();
	}

	columnsString += ",taskId";
	valuesString += "," + to_string(parentTaskId);
	columnsString += ",status";
	valuesString += "," + to_string((uint64_t)newEvent.getStatus());
	columnsString += ",recurringIndex";
	valuesString += "," + to_string(parent->getRecurringIndex());

	string insertString =
	"insert into events (" +
	columnsString + ") values(" + valuesString + ")";

	database->execute(insertString);
	eventInsertedRowId = database->lastInsertRowId();

	return eventInsertedRowId;
}

void AppDB::updateEvent(uint64_t eventId, const Event & updatedEvent)
{
	string updateString = "update events set ";

	updateString += "startTime = " + to_string(updatedEvent.getStartTime());
	updateString += ",duration = " + to_string(updatedEvent.getDuration());

	uint64_t parentTaskId;
	auto parent = updatedEvent.getParent();
	parentTaskId = parent->getTaskId();
	if(!parent->getRecurranceParent().expired())
	{
		auto recurringParent = parent->getRecurranceParent().lock();
		parentTaskId = recurringParent->getTaskId();
	}

	updateString += ",taskId = " + to_string(parentTaskId);
	updateString += ",status = " + to_string((uint64_t)updatedEvent.getStatus());
	updateString += ",recurringIndex = " + to_string(parent->getRecurringIndex());

	updateString += " where eventId = " + to_string(eventId);

	database->execute(updateString);
}

void AppDB::removeEvent(uint64_t eventId)
{
	string deleteString =
	"delete from events where eventId = " + to_string(eventId);

	database->execute(deleteString);
}

string AppDB::getCurrentVersion()
{
    return currentVersion;
}

}
}
