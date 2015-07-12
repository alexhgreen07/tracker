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
	TrackerApp(string dbName);
    bool start();
private:
    string dbName;
	Database::DatabaseSqlite3 mysqlDB;
	AppDB db;
	AppClock apiClock;
	AppApi api;
    Server server;
};
    
}
}



#endif
