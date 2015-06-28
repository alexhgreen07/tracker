#include "app.hpp"

namespace Tracker
{
namespace Application
{

TrackerApp::TrackerApp(string dbName) :
	dbName(dbName),
	db(mysqlDB),
	api(db),
	server(api)
{}
	
bool TrackerApp::start()
{
	mysqlDB.open(dbName);

	//TODO: remove, for testing only
	db.initializeNewDatabase();

    return server.start();
}

}
}
