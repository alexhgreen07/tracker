#ifndef __APP_DB_HPP__
#define __APP_DB_HPP__

#include <string>
#include <map>
#include <stdlib.h>

#include <database.hpp>
#include <task.hpp>

namespace Tracker
{
namespace Application
{

class AppDB
{
public:
    AppDB(Database::Database & database);
    
    void updateDatabase();
    void initializeNewDatabase();
    void createVersionTable();
    void createTasksTable();
    std::shared_ptr<std::map<uint64_t, std::shared_ptr<Core::Task>>> getTasks();
    uint64_t insertTask(const Core::Task & newTask);
    void updateTask(uint64_t taskId, Core::Task & task);
    void removeTask(uint64_t taskId);
    std::string getCurrentVersion();
private:
    Database::Database & database;
    std::string currentVersion;

    uint64_t insertTask(const Core::Task & newTask, unsigned int recurringParent);
};

}
}

#endif
