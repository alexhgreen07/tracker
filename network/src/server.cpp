#include "server.hpp"

#include "api.hpp"

namespace Tracker
{
namespace Network
{

Server::Procedures::Procedures()
{
	procedurePointers["sayHello"] = &Api::sayHello;
    notPointers["notifyServer"] = &Api::notifyServer;
}

Server::Server() :
    http(8080,"./ui"),
    serv("A Server Instancename", "ui/procedures.json", procedures.procedurePointers, procedures.notPointers, &http)
{
    
}

bool Server::start()
{
    return serv.startServer();
}

}
}