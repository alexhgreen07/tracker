#include "app.hpp"

namespace Tracker
{
namespace Application
{

TrackerApp::TrackerApp() :
	db(mysqlDB),
	api(db),
	server(api)
{
	
}
	
bool TrackerApp::start()
{
	//TODO: remove, for testing only
	mysqlDB.open(":memory:");
	db.initializeNewDatabase();

    return server.start();
}

}
}
