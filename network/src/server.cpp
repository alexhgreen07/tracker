#include "server.hpp"

namespace Tracker
{
namespace Network
{

Server::Server(const std::shared_ptr<Api> & api) :
	api(api),
    http(8080,"./ui"),
    serv("A Server Instancename", "ui/procedures.json", api->getProcedures(), api->getNotifications(), &http)
{}

bool Server::start()
{
    return serv.startServer();
}

}
}
