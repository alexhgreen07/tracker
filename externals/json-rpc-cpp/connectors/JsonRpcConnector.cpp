/**
 * @file JsonRpcConnector.cpp
 * @date 22.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcConnector.cpp provides 
 */

#include "JsonRpcConnector.h"
#include <iostream>

namespace jsonrpc {

JsonRpcConnector::JsonRpcConnector() {
}

    JsonRpcConnector::JsonRpcConnector(JsonRpcRequestHandler* fp)
    {
    	this->pp = fp;

    }

    bool JsonRpcConnector::onRequest(const char* request, void* addInfo)
    {
    	string response;
    	if(this->pp != NULL) {
			this->pp->handleRequest(request,response);
			this->sendResponse(response,addInfo);
			return true;
    	} else {
    		return false;
    	}
    }



}
