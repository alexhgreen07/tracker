/**
 * @file JsonRpcServer.h
 * @date 22.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcServer.h provides 
 */

#ifndef JSONRPCSERVER_H_
#define JSONRPCSERVER_H_

#include "connectors/JsonRpcConnector.h"
#include "JsonRpcRequestHandler.h"

namespace jsonrpc {


typedef map<string,jsonRequestPointer> JsonMethods;
typedef map<string,jsonNotificationPointer> JsonNotifications;


/**
 * This class should provide a very comfortable Possibility to create a new JsonRpcServer instance.
 * All methods are parsed automatically from a given json-methodfile.
 * The necessary callback-functions are set automatically.
 */
class JsonRpcServer {
private:
	JsonRpcRequestHandler* handler;
	JsonRpcConnector* connection;
	string fileName;

	int parseFromFile(string filename);

public:
	JsonRpcServer(const char* instancename, const char* filename, JsonMethods& procedurePointers, JsonNotifications& notificationPointers, JsonRpcConnector* connection, JsonRpcAuthenticationManager* authMgr = NULL);
	virtual ~JsonRpcServer();

	bool startServer();
	bool stopServer();

	const map<string,JsonRpcProcedure*>& getProcedures();

    JsonRpcConnector *getConnection() const;
    const string& getFlieName() const;
    JsonRpcRequestHandler *getHandler() const;
    void setConnection(JsonRpcConnector *connection);
    void setFileName(string& fileName);
    void setHandler(JsonRpcRequestHandler *handler);

};

} /* namespace jsonrpc */
#endif /* JSONRPCSERVER_H_ */
