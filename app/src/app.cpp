#include "app.hpp"

namespace Tracker
{
namespace Application
{

bool TrackerApp::start()
{
    return server.start();
}

}
}