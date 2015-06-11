#include "server.hpp"

namespace Tracker
{
namespace Network
{

static void sayHello(const Json::Value& request, Json::Value& response) {
    response = "Hello: " + request["name"].asString();
}

static void notifyServer(const Json::Value& request) {
    cout << "server received some Notification" << endl;
}

Server::Procedures::Procedures()
{
    procedurePointers["sayHello"] = &sayHello;
    notPointers["notifyServer"] = &notifyServer;
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