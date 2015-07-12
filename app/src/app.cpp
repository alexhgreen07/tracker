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

TrackerApp::TrackerApp(string dbName) :
	dbName(dbName),
	db(mysqlDB),
	api(db,apiClock),
	server(api)
{}
	
bool TrackerApp::start()
{
	mysqlDB.open(dbName);
	db.updateDatabase();

    return server.start();
}

}
}
