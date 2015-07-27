#ifndef __APP_HPP__
#define __APP_HPP__

#include <server.hpp>
#include <app_db.hpp>
#include <app_api.hpp>

namespace Tracker
{
namespace Application
{

using namespace Network;

class AppClock : public AppApi::Clock
{
	uint64_t getNowTimestamp() override;
};

class TrackerApp
{
public:
	TrackerApp(
			string dbName,
			const std::shared_ptr<Database::DatabaseSqlite3> & sqliteDB,
			const std::shared_ptr<AppDB> & db,
			const std::shared_ptr<Server> & server);
    bool start();
private:
    string dbName;
    std::shared_ptr<Database::DatabaseSqlite3> sqliteDB;
    std::shared_ptr<AppDB> db;
    std::shared_ptr<Server> server;
};

std::shared_ptr<TrackerApp> buildTrackerApp(string dbName);
    
}
}



#endif
