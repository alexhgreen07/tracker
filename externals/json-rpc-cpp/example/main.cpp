/**
 * @file main.cpp
 * @date 01.08.2011
 * @author Peter Spiess-Knafl
 * @brief main.cpp
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <json-rpc-cpp/json-rpc.h>

using namespace jsonrpc;

void sayHello(const Json::Value& request, Json::Value& response) {
	response = "Hello: " + request["name"].asString();
}

void notifyServer(const Json::Value& request) {
	cout << "server received some Notification" << endl;
}

int main() {
	JsonMethods procedurePointers;
	JsonNotifications notPointers;

	procedurePointers["sayHello"] = &sayHello;
	notPointers["notifyServer"] = &notifyServer;


	HTTPConnector http(8080,"./res");

	JsonRpcServer serv("A Server Instancename", "res/procedures.json", procedurePointers, notPointers, &http);
	if(serv.startServer()) {
		cout << "Server started successfully" << endl;
		getchar();
		serv.stopServer();
	} else {
		cout << "Error starting Server" << endl;
	}


	//"{\"jsonrpc\":2.0,\"method\":\"sayHello\",\"id\":1,\"params\":{\"name\":\"peter\"}}"

}
