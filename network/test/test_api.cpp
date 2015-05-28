#include <string>
#include <iostream>

#include <CppUTest/TestHarness.h>

#include <json-rpc.h>

using namespace jsonrpc;

void sayHello(const Json::Value& request, Json::Value& response) {
    response = "Hello: " + request["name"].asString();
}

void notifyServer(const Json::Value& request) {
    cout << "server received some Notification" << endl;
}

TEST_GROUP(NetworkApiGroup)
{
};

TEST(NetworkApiGroup, BasicOpen)
{
    JsonMethods procedurePointers;
    JsonNotifications notPointers;
    
    procedurePointers["sayHello"] = &sayHello;
    notPointers["notifyServer"] = &notifyServer;
    
    HTTPConnector http(8080,"./res");
    
    JsonRpcServer serv("A Server Instancename", "res/procedures.json", procedurePointers, notPointers, &http);
    CHECK(serv.startServer());
}