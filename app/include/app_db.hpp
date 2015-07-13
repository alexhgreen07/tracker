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
namespace Application
{

class AppDB
{
public:
    AppDB(const std::shared_ptr<Database::Database> & database);
    
    void updateDatabase();

    void initializeNewDatabase();

    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Core::Task>>> getTasks();
    uint64_t insertTask(const Core::Task & newTask);
    void updateTask(uint64_t taskId, Core::Task & task);
    void removeTask(uint64_t taskId);

    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Core::Event>>> getLoggedEvents();
    uint64_t insertEvent(const Core::Event & newEvent);
    void updateEvent(uint64_t eventId, const Core::Event & updatedEvent);
    void removeEvent(uint64_t eventId);

    std::string getCurrentVersion();
private:
    std::shared_ptr<Database::Database> database;
    std::string currentVersion;

    void createVersionTable();
    void createTasksTable();
    void createEventsTable();
};

}
}

#endif
