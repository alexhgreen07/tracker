/**
 * @file HTTPConnector.cpp
 * @date 31.07.2011
 * @author Peter Spiess-Knafl
 * @brief HTTPConnector.cpp provides 
 */

#include "HTTPConnector.h"
#include <string>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
using namespace std;

namespace jsonrpc {

static void *callback(enum mg_event event, struct mg_connection *conn,
		const struct mg_request_info *request_info) {

	char* readBuffer = NULL;
	int postSize = 0;
	HTTPConnector* _this = (HTTPConnector*) request_info->user_data;


	if(event == MG_NEW_REQUEST) {

		if(strcmp(request_info->request_method,"GET") == 0) {
			//Mark the request as unprocessed.
			return NULL;
		} else if(strcmp(request_info->request_method,"POST") == 0) {
			//get size of postData
			sscanf(mg_get_header(conn,"Content-Length"),"%d",&postSize);
			readBuffer = (char*) malloc(sizeof(char)*(postSize+1));
			mg_read(conn,readBuffer,postSize);
			_this->onRequest(readBuffer,conn);
			free(readBuffer);

			//Mark the request as processed by our handler.
			return (void*)"";
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
}

HTTPConnector::HTTPConnector(int port, const string& resPath) : JsonRpcConnector() {
	this->port = port;
	this->ctx = NULL;
	this->resPath = resPath;
}

bool HTTPConnector::sendResponse(string & response, void *addInfo)
{
	struct mg_connection* conn = (struct mg_connection*)addInfo;
	if(mg_printf(conn,"HTTP/1.1 200 OK\r\n\r\n %s",response.c_str()) > 0) {
		return true;
	} else {
		return false;
	}
}

bool HTTPConnector::startPolling()
{
	char port[6];
	sprintf(port,"%d",this->port);
	const char *options[] = { "document_root",this->resPath.c_str(), "listening_ports", port, NULL };
	this->ctx = mg_start(&callback, this, options);
	if(this->ctx != NULL) {
		return true;
	} else {
		return false;
	}
}

bool HTTPConnector::stopPolling()
{
	mg_stop(this->ctx);
	return true;
}

HTTPConnector::~HTTPConnector() {
	mg_stop(this->ctx);
}


}
