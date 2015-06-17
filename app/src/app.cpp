#include "app.hpp"

namespace Tracker
{
namespace Application
{

TrackerApp::TrackerApp() :
	server(api)
{
	
}
	
bool TrackerApp::start()
{
    return server.start();
}

}
}