/**
 * @file JsonRpcServer.cpp
 * @date 22.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcServer.cpp provides 
 */

#include "JsonRpcServer.h"
#include "JsonRpcErrors.h"
#include <fstream>

extern jsonrpc::JsonRpcRequestHandler* handler;
extern jsonrpc::JsonRpcConnector* connector;

namespace jsonrpc {

JsonRpcConnector *JsonRpcServer::getConnection() const {
	return connection;
}

const string& JsonRpcServer::getFlieName() const {
	return fileName;
}

JsonRpcRequestHandler *JsonRpcServer::getHandler() const {
	return handler;
}

void JsonRpcServer::setConnection(JsonRpcConnector *connection) {
	this->connection = connection;
}

void JsonRpcServer::setFileName(string& fileName) {
	this->fileName = fileName;
}

JsonRpcServer::JsonRpcServer(const char* instancename, const char* filename,
		JsonMethods& methodPointers, JsonNotifications& notificationPointers, JsonRpcConnector *con,
		JsonRpcAuthenticationManager *authMgr) {

	this->handler = new JsonRpcRequestHandler(instancename);
	this->fileName = filename;
	JsonRpcProcedure* proc;
	Json::Reader reader;
	Json::Value val, line;
	string value;
	ifstream myfile;
	char tmp[500];

	myfile.open(filename, std::ifstream::in);
	while (!myfile.eof()) {
		myfile.getline(tmp, 255);
		value.append(tmp);

	}
	myfile.close();

	if (!reader.parse(value, val)) {
		throw std::string("Error while parsing json-method file: "
				+ this->fileName);
	}

	JsonNotifications::iterator it_notifications;
	JsonMethods::iterator it_methods;
	for (unsigned int i = 0; i < val.size(); i++) {
		line = val[i];
		proc = new JsonRpcProcedure(line);
		if(proc->getProcedureType() == RPC_METHOD) {
			it_methods = methodPointers.find(proc->getProcedureName());
			if(it_methods != methodPointers.end()) {
				proc->setMethodPointer(it_methods->second);
			}
		} else {
			it_notifications = notificationPointers.find(proc->getProcedureName());
			if(it_notifications != notificationPointers.end()) {
				proc->setNotificationPointer(it_notifications->second);
			}
		}
		this->handler->addProcedure(proc);
	}
	this->connection = con;
	this->connection->setHandler(this->handler);
}

bool JsonRpcServer::startServer() {
	return this->connection->startPolling();
}

bool JsonRpcServer::stopServer() {
	return this->connection->stopPolling();
}

const map<string, JsonRpcProcedure*>& JsonRpcServer::getProcedures() {
	return this->handler->getProcedures();
}

JsonRpcServer::~JsonRpcServer() {
	delete this->handler;
}

} /* namespace jsonrpc */
