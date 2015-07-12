#include "app.hpp"

#include <sys/time.h>

namespace Tracker
{
namespace Application
{

uint64_t AppClock::getNowTimestamp()
{
	return time(NULL);
}

TrackerApp::TrackerApp(
		string dbName,
		const std::shared_ptr<Database::DatabaseSqlite3> & sqliteDB,
		const std::shared_ptr<AppDB> & db,
		const std::shared_ptr<Server> & server) :
	dbName(dbName),
	sqliteDB(sqliteDB),
	db(db),
	server(server)
{}
	
bool TrackerApp::start()
{
	sqliteDB->open(dbName);
	db->updateDatabase();

    return server->start();
}

std::shared_ptr<TrackerApp> buildTrackerApp(string dbName)
{
	auto sqliteDB = std::make_shared<Database::DatabaseSqlite3>();
	auto db = std::make_shared<AppDB>(sqliteDB);
	auto apiClock = std::make_shared<AppClock>();
	auto api = std::make_shared<AppApi>(db,apiClock);
	auto server = std::make_shared<Server>(api);

	auto app = std::make_shared<TrackerApp>(
		dbName,
		sqliteDB,
		db,
		server
	);

	return app;
}

}
}
