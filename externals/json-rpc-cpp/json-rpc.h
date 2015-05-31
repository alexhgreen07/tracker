/**
 * @file json-rpc.h
 * @date 02.08.2011
 * @author Peter Spiess-Knafl
 * @brief json-rpc.h
 *
 * This file is meant to include all necessary .h files of this framework.
 */

#ifndef JSONRPCCPP_H_
#define JSONRPCCPP_H_

//The only class you will need.
#include "JsonRpcServer.h"

//For error handling and catching Exceptions.
#include "JsonRpcErrors.h"

#include "connectors/JsonRpcConnector.h"

//The currently only Connector we have in this framework.
#include "connectors/http/HTTPConnector.h"


#endif /* JSONRPCCPP_H_ */
