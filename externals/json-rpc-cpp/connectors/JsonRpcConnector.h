/**
 * @file JsonRpcConnector.h
 * @date 22.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcConnector.h
 */

#ifndef JSONRPCCONNECTOR_H_
#define JSONRPCCONNECTOR_H_

#include <string>
#include "../JsonRpcRequestHandler.h"

using namespace std;

namespace jsonrpc {

/**
 * This Abstract class is used for generalization purpose, to keep the JsonRpcServer generic.
 * The methods need to be implemented very as described at each method.
 */
class JsonRpcConnector {

private:
	JsonRpcRequestHandler* pp;
public:
	JsonRpcConnector();
	JsonRpcConnector(JsonRpcRequestHandler* fp);



	//virtual ~JsonRpcConnector();

	/**
	 * This method should signal the Connector to start waiting for requests, in any way that is appropriate for the derived connector class.
	 * If something went wrong, this method should return false, otherwise true.
	 */
	virtual bool startPolling() = 0;
	/**
	 * This method should signal the Connector to stop waiting for requests, in any way that is appropriate for the derived connector class.
	 * If something went wrong, this method should return false, otherwise true.
	 */
	virtual bool stopPolling() = 0;

	/**
	 * This method should send a response to the client in any way that is appropriate for the derived connector class.
	 * @param response - the response that should be send to the client
	 * @param addInfo - additional Info, that the Connector might need for responding.
	 * @return returns true on success, false otherwise
	 */
	bool virtual sendResponse(string& response, void* addInfo = NULL) = 0;

	/**
	 * This method must be called, when a request is recognised. It will do everything else for you (including sending the response).
	 * @param request - the request that has been recognised.
	 * @param addInfo - additional Info, that the Connector might need for responding.
	 */
	bool onRequest(const char* request, void* addInfo = NULL);

	void setHandler(JsonRpcRequestHandler* handler) {
		this->pp = handler;
	}

	JsonRpcRequestHandler* getHandler() {
		return this->pp;
	}


};

} /* namespace jsonrpc */
#endif /* JSONRPCCONNECTOR_H_ */
