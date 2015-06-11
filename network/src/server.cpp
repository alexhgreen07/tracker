#include "server.hpp"

namespace Tracker
{
namespace Network
{

Server::Server() :
    http(8080,"./ui"),
    serv("A Server Instancename", "ui/procedures.json", api.procedurePointers, api.notPointers, &http)
{}

bool Server::start()
{
    return serv.startServer();
}

}
}