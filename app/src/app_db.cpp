#include "app_db.hpp"

namespace Tracker
{
namespace Application
{

AppDB::AppDB(Database::Database & database) :
    database(database),
    currentVersion("0")
{}

void AppDB::updateDatabase()
{
	bool shouldInitialize = false;

	try
	{
		auto tasksTable = database.select("select version from version");

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
    database.execute("create table version (version int)");
    
    createVersionTable();
    createTasksTable();
}

void AppDB::createVersionTable()
{
    database.execute("insert into version values (" + currentVersion + ")");
}

void AppDB::createTasksTable()
{
    std::string createSql = "create table tasks (";
    createSql += "taskId integer primary key asc";
    createSql += ",name text";
    createSql += ",earliestStartTime real";
    createSql += ",latestEndTime real";
    createSql += ",duration real";
    createSql += ");";
    
    database.execute(createSql);
}

std::shared_ptr<std::map<uint64_t, Core::Task>> AppDB::getTasks()
{
    auto tasksTable = database.select("select taskId, name, earliestStartTime, latestEndTime, duration from tasks");
    auto tasks = std::make_shared<std::map<uint64_t, Core::Task>>();
    
    for(unsigned int i = 0; i < tasksTable->size(); i++)
    {
        Core::Task nextTask;
        auto row = tasksTable->at(i);

        uint64_t taskId = (uint64_t)atoll(row[0].c_str());
        nextTask.setName(row[1]);
        nextTask.setEarliestStartTime(atoi(row[2].c_str()));
        nextTask.setLatestEndTime(atoi(row[3].c_str()));
        nextTask.setDuration(atoi(row[4].c_str()));
        
        tasks->insert(std::pair<uint64_t, Core::Task>(taskId,nextTask));
    }
    
    return tasks;
}

uint64_t AppDB::insertTask(const Core::Task & newTask)
{
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
    
    std::string insertString =
    "insert into tasks (" +
    columnsString + ") values(" + valuesString + ")";
    
    database.execute(insertString);
    return database.lastInsertRowId();
}

void AppDB::updateTask(uint64_t taskId, Core::Task & task)
{
    std::string updateString = "update tasks set ";
    
    updateString += "name = '" + task.getName() + "'";
    updateString += ",earliestStartTime = " + std::to_string(task.getEarliestStartTime());
    updateString += ",latestEndTime = " + std::to_string(task.getLatestEndTime());
    updateString += ",duration = " + std::to_string(task.getDuration());
    
    updateString += " where taskId = " + std::to_string(taskId);
    
    database.execute(updateString);
}

void AppDB::removeTask(uint64_t taskId)
{
    std::string deleteString =
    "delete from tasks where taskId = " + std::to_string(taskId);
    
    database.execute(deleteString);
}

std::string AppDB::getCurrentVersion()
{
    return currentVersion;
}

}
}
