#include "server.hpp"

namespace Tracker
{
namespace Network
{

Server::Server() :
    http(8080,"./ui"),
    serv("A Server Instancename", "ui/procedures.json", procedures.procedurePointers, procedures.notPointers, &http)
{}

bool Server::start()
{
    return serv.startServer();
}

}
}