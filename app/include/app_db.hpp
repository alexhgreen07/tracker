#ifndef __APP_DB_HPP__
#define __APP_DB_HPP__

#include <string>
#include <map>
#include <stdlib.h>

#include <database.hpp>
#include <task.hpp>
#include <event.hpp>

namespace Tracker
{

using namespace Core;

namespace Application
{

class AppDB
{
public:
    AppDB(const std::shared_ptr<Database::Database> & database);
    
    std::shared_ptr<Database::Database> getDatabase();

    void updateDatabase();

    void initializeNewDatabase();

    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Task>>> getTasks();
    uint64_t insertTask(const Task & newTask);
    void updateTask(uint64_t taskId, Task & task);
    void removeTask(uint64_t taskId);

    void updateRecurringTaskStatus(uint64_t taskId, uint64_t recurringIndex, Task::Status status);

    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Event>>> getLoggedEvents();
    uint64_t insertEvent(const Event & newEvent);
    void updateEvent(uint64_t eventId, const Event & updatedEvent);
    void removeEvent(uint64_t eventId);

    std::string getCurrentVersion();
private:
    std::shared_ptr<Database::Database> database;
    std::string currentVersion;

    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Task>>> tasks;
    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Event>>> events;

    void createVersionTable();
    void createTasksTable();
    void createRecurringTasksStatusTable();
    void createEventsTable();

    uint64_t insertRecurringTaskStatus(uint64_t taskId, uint64_t recurringIndex, Task::Status status);
    void removeRecurringTaskStatus(uint64_t taskId);
};

}
}

#endif
