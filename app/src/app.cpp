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
    return server.start();
}

}
}