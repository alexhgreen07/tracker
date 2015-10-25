#include "app.hpp"

#include <iostream>
#include <sys/time.h>

using std::cout;
using std::endl;

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
	cout << "Starting application with database '" << dbName << "'" << endl;
	sqliteDB->open(dbName);
	cout << "Updating database" << endl;
	db->updateDatabase();

	cout << "Starting server" << endl;
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
