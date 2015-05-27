/**
 * @file HTTPConnector.h
 * @date 31.07.2011
 * @author Peter Spiess-Knafl
 * @brief HTTPConnector.h
 */

#ifndef HTTPCONNECTOR_H_
#define HTTPCONNECTOR_H_

#include "../JsonRpcConnector.h"
#include "mongoose.h"

namespace jsonrpc {

/**
 * Typedefinition to differ between Text and Binary ressources
 */
typedef enum {TEXT, BINARY} ResourceType;


/**
 * This class provides an embedded HTTP Server, based on Mongoose, to handle incoming Requests and send HTTP 1.1
 * valid responses.
 * Note that this class will always send HTTP-Status 200, even though an JSON-RPC Error might have occurred. Please
 * always check for the JSON-RPC Error Header.
 *
 * Query String Parameters:
 * 	You can send Query String Parameters in a GET Request. E.g.: http://testserver:port?someParameter
 *
 * 	Possible Parameters:
 * 		jsonrpcmethods: Will send you the json-rpc-cpp Configuration file with all method descriptions
 * 		html: Will read a defined (see Constructor) HTML File.
 */
class HTTPConnector: public jsonrpc::JsonRpcConnector {
private:
	int port;
	struct mg_context *ctx;
	string resPath;



public:

	/**
	 * @param port - the port under which the server should run
	 * @param GETresources - a path to a directory that should contain the GET-"able" resources.
	 */
	HTTPConnector(int port, const string& GETresources);

	bool startPolling();
	bool stopPolling();

	bool sendResponse(string& response, void* addInfo = NULL);

	virtual ~HTTPConnector();

	const string& getResPath() {
		return this->resPath;
	}

};
}
#endif /* HTTPCONNECTOR_H_ */
