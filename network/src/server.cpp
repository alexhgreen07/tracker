#include "server.hpp"

static void sayHello(const Json::Value& request, Json::Value& response) {
    response = "Hello: " + request["name"].asString();
}

static void notifyServer(const Json::Value& request) {
    cout << "server received some Notification" << endl;
}

Server::Server() :
    http(8080,"./res"),
    serv("A Server Instancename", "res/procedures.json", procedurePointers, notPointers, &http)
{
    procedurePointers["sayHello"] = &sayHello;
    notPointers["notifyServer"] = &notifyServer;
}

bool Server::start()
{
    return serv.startServer();
}