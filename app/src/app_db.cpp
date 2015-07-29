#include "app_db.hpp"

#include <sstream>

namespace Tracker
{
namespace Application
{

AppDB::AppDB(const std::shared_ptr<Database::Database> & database) :
    database(database),
    currentVersion("0")
{}

std::shared_ptr<Database::Database> AppDB::getDatabase()
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
    createEventsTable();
}

void AppDB::createVersionTable()
{
    database->execute("insert into version values (" + currentVersion + ")");
}

void AppDB::createTasksTable()
{
    std::string createSql = "create table tasks (";
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

void AppDB::createEventsTable()
{
	std::string createSql = "create table events (";
	createSql += "eventId integer primary key asc";
	createSql += ",startTime integer";
	createSql += ",duration integer";
	createSql += ",taskId integer";
	createSql += ",recurringIndex integer";
	createSql += ");";

	database->execute(createSql);
}

std::shared_ptr<std::map<uint64_t, std::shared_ptr<Core::Task>>> AppDB::getTasks()
{
    auto tasksTable = database->select("select taskId, name, earliestStartTime, latestEndTime, duration, status, recurringPeriod, recurringLateOffset from tasks");
    tasks = std::make_shared<std::map<uint64_t, std::shared_ptr<Core::Task>>>();
    
    for(unsigned int i = 0; i < tasksTable->size(); i++)
    {
        auto nextTask = std::make_shared<Core::Task>();
        auto row = tasksTable->at(i);

        uint64_t value;
        uint64_t taskId;

        std::istringstream input_stream(row[0]);
        input_stream >> taskId;
        nextTask->setTaskId(taskId);

        nextTask->setName(row[1]);
        input_stream = std::istringstream(row[2]);
        input_stream >> value;
        nextTask->setEarliestStartTime(value);

        input_stream = std::istringstream(row[3]);
        input_stream >> value;
        nextTask->setLatestEndTime(value);

        input_stream = std::istringstream(row[4]);
		input_stream >> value;
        nextTask->setDuration(value);

        input_stream = std::istringstream(row[5]);
		input_stream >> value;
		nextTask->setStatus((Core::Task::Status)value);

        uint64_t recurrancePeriod;
        uint64_t recurranceLateOffset;
        input_stream = std::istringstream(row[6]);
		input_stream >> recurrancePeriod;
		input_stream = std::istringstream(row[7]);
		input_stream >> recurranceLateOffset;
        nextTask->setRecurranceParameters(recurrancePeriod,recurranceLateOffset);
		
		(*tasks)[taskId] = nextTask;
    }
    
    return tasks;
}

uint64_t AppDB::insertTask(const Core::Task & newTask)
{
	uint64_t taskInsertedRowId = 0;

    std::string columnsString = "";
    std::string valuesString = "";
	
	columnsString += "name";
	valuesString += "'" + newTask.getName() + "'";
    columnsString += ",earliestStartTime";
    valuesString += "," + std::to_string(newTask.getEarliestStartTime());
    columnsString += ",latestEndTime";
    valuesString += "," + std::to_string(newTask.getLatestEndTime());
    columnsString += ",duration";
    valuesString += "," + std::to_string(newTask.getDuration());
	columnsString += ",status";
    valuesString += "," + std::to_string((uint64_t)newTask.getStatus());
    columnsString += ",recurringPeriod";
	valuesString += "," + std::to_string(newTask.getRecurringPeriod());
	columnsString += ",recurringLateOffset";
	valuesString += "," + std::to_string(newTask.getRecurringLateOffset());
    
    std::string insertString =
    "insert into tasks (" +
    columnsString + ") values(" + valuesString + ")";
    
    database->execute(insertString);
    taskInsertedRowId = database->lastInsertRowId();

    getTasks();

    return taskInsertedRowId;
}

void AppDB::updateTask(uint64_t taskId, Core::Task & task)
{
    std::string updateString = "update tasks set ";
    
    updateString += "name = '" + task.getName() + "'";
    updateString += ",earliestStartTime = " + std::to_string(task.getEarliestStartTime());
    updateString += ",latestEndTime = " + std::to_string(task.getLatestEndTime());
    updateString += ",duration = " + std::to_string(task.getDuration());
    updateString += ",status = " + std::to_string((uint64_t)task.getStatus());
    updateString += ",recurringPeriod = " + std::to_string(task.getRecurringPeriod());
    updateString += ",recurringLateOffset = " + std::to_string(task.getRecurringLateOffset());
    
    updateString += " where taskId = " + std::to_string(taskId);
    
    database->execute(updateString);

    getTasks();
}

void AppDB::removeTask(uint64_t taskId)
{
    std::string deleteString =
    "delete from tasks where taskId = " + std::to_string(taskId);
    
    database->execute(deleteString);

    getTasks();
}

std::shared_ptr<std::map<uint64_t, std::shared_ptr<Core::Event>>> AppDB::getLoggedEvents()
{
	auto eventsTable = database->select("select eventId, startTime, duration, taskId, recurringIndex from events");
	events = std::make_shared<std::map<uint64_t, std::shared_ptr<Core::Event>>>();

	if(!tasks)
	{
		getTasks();
	}

	for(unsigned int i = 0; i < eventsTable->size(); i++)
	{
		auto nextEvent = std::make_shared<Core::Event>();
		auto row = eventsTable->at(i);

		//TODO: add parent task

		uint64_t value;
		uint64_t eventId;
		uint64_t recurringIndex;

		std::istringstream input_stream(row[0]);
		input_stream >> eventId;
		nextEvent->setEventId(eventId);

		input_stream = std::istringstream(row[1]);
		input_stream >> value;
		nextEvent->setStartTime(value);

		input_stream = std::istringstream(row[2]);
		input_stream >> value;
		nextEvent->setDuration(value);

		input_stream = std::istringstream(row[3]);
		input_stream >> value;
		
		auto parentTaskAtId = (*tasks)[value];

		input_stream = std::istringstream(row[4]);
		input_stream >> recurringIndex;

		if(parentTaskAtId->getRecurringTaskCount() > 0)
		{
			nextEvent->setParent(parentTaskAtId->getRecurringChild(recurringIndex));
		}
		else
		{
			nextEvent->setParent(parentTaskAtId);
		}

		nextEvent->setStatus(Core::Event::Status::Logged);

		(*events)[eventId] = nextEvent;
	}

	return events;
}

uint64_t AppDB::insertEvent(const Core::Event & newEvent)
{
	uint64_t eventInsertedRowId = 0;

	std::string columnsString = "";
	std::string valuesString = "";

	columnsString += "startTime";
	valuesString += std::to_string(newEvent.getStartTime());
	columnsString += ",duration";
	valuesString += "," + std::to_string(newEvent.getDuration());

	uint64_t parentTaskId;
	auto parent = newEvent.getParent();
	parentTaskId = parent->getTaskId();
	if(!parent->getRecurranceParent().expired())
	{
		auto recurringParent = parent->getRecurranceParent().lock();
		parentTaskId = recurringParent->getTaskId();
	}

	columnsString += ",taskId";
	valuesString += "," + std::to_string(parentTaskId);
	columnsString += ",recurringIndex";
	valuesString += "," + std::to_string(parent->getRecurringIndex());

	std::string insertString =
	"insert into events (" +
	columnsString + ") values(" + valuesString + ")";

	database->execute(insertString);
	eventInsertedRowId = database->lastInsertRowId();

	return eventInsertedRowId;
}

void AppDB::updateEvent(uint64_t eventId, const Core::Event & updatedEvent)
{
	std::string updateString = "update events set ";

	updateString += "startTime = " + std::to_string(updatedEvent.getStartTime());
	updateString += ",duration = " + std::to_string(updatedEvent.getDuration());

	uint64_t parentTaskId;
	auto parent = updatedEvent.getParent();
	parentTaskId = parent->getTaskId();
	if(!parent->getRecurranceParent().expired())
	{
		auto recurringParent = parent->getRecurranceParent().lock();
		parentTaskId = recurringParent->getTaskId();
	}

	updateString += ",taskId = " + std::to_string(parentTaskId);
	updateString += ",recurringIndex = " + std::to_string(parent->getRecurringIndex());

	updateString += " where eventId = " + std::to_string(eventId);

	database->execute(updateString);
}

void AppDB::removeEvent(uint64_t eventId)
{
	std::string deleteString =
	"delete from events where eventId = " + std::to_string(eventId);

	database->execute(deleteString);
}

std::string AppDB::getCurrentVersion()
{
    return currentVersion;
}

}
}
