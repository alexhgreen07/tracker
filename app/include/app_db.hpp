#ifndef __APP_DB_HPP__
#define __APP_DB_HPP__

#include <string>
#include <map>
#include <stdlib.h>

#include <database.hpp>
#include <task.hpp>
#include <event.hpp>

using std::shared_ptr;
using std::numeric_limits;

namespace Tracker
{

using namespace Core;

namespace Application
{

class AppDB
{
public:
	struct AppData
	{
		shared_ptr<std::map<uint64_t, shared_ptr<Task>>> tasks;
		shared_ptr<std::map<uint64_t, shared_ptr<Event>>> loggedEvents;
	};

    AppDB(const shared_ptr<Database::Database> & database);
    
    shared_ptr<Database::Database> getDatabase();

    void updateDatabase();

    void initializeNewDatabase();

    uint64_t insertTask(const Task & newTask);
    void updateTask(uint64_t taskId, Task & task);
    void removeTask(uint64_t taskId);

    void updateRecurringTaskStatus(uint64_t taskId, uint64_t recurringIndex, Task::Status status);

    uint64_t insertEvent(const Event & newEvent);
    void updateEvent(uint64_t eventId, const Event & updatedEvent);
    void removeEvent(uint64_t eventId);

    shared_ptr<AppData> getAppData(uint64_t startTime = 0, uint64_t endTime = numeric_limits<uint64_t>::max());

    std::string getCurrentVersion();
private:
    shared_ptr<Database::Database> database;
    std::string currentVersion;

    void createVersionTable();
    void createTasksTable();
    void createRecurringTasksStatusTable();
    void createEventsTable();

    shared_ptr<std::map<uint64_t, shared_ptr<Task>>> getTasks(uint64_t startTime = 0, uint64_t endTime = numeric_limits<uint64_t>::max());
    shared_ptr<std::map<uint64_t, shared_ptr<Event>>> getLoggedEvents(
    		shared_ptr<std::map<uint64_t, shared_ptr<Task>>> & tasks,
			uint64_t startTime = 0, uint64_t endTime = numeric_limits<uint64_t>::max());

    uint64_t insertRecurringTaskStatus(uint64_t taskId, uint64_t recurringIndex, Task::Status status);
    void removeRecurringTaskStatus(uint64_t taskId);
};

}
}

#endif
