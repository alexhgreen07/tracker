/**
 * @file JsonRpcAuthenticationManager.h
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcAuthenticationManager.h
 */

#ifndef JSONRPCAUTHENTICATIONMANAGER_H_
#define JSONRPCAUTHENTICATIONMANAGER_H_

#include "json/json.h"

namespace jsonrpc {

/**
 *	This abstract class is managing the Authentication process for the JsonRpcRequestHandler.
 *	The JsonRpcRequestHandler will call checkPermission on each Request.
 *
 *	It is very important that these virtual/abstract Methods are implemented properly. -> Very high Crash risk!!!
 */
class JsonRpcAuthenticationManager {
public:

	/**
	 * This method must be defined in any derived class from JsonRpcAuthenticationManager.
	 * @param authentication - should contain the current authentication header of the incoming request.
	 * @param method - the method that is planned to be processed.
	 * @return ERROR_NO if permission should be granted. returns ERROR_PERMISSION_DENIED if no permission should be granted.
	 * 		If something was wrong with the authentication header, it should return ERROR_MALLFORMED_AUTHENTICATION_HEADER.
	 */
	virtual int checkPermission(const Json::Value &authentication, const string &method) = 0;

	/**
	 * This method must be defined in any derived class from JsonRpcAuthenticationManager.
	 * @param authentication - will contain the current authentication Header of the incoming request.
	 * @param returnValue - a reference to a valid Json::Value, which will contain the new authHeader.
	 */
	virtual void processAuthentication(const Json::Value &authentication, Json::Value &returnValue) = 0;

	//virtual ~JsonRpcAuthenticationManager() = 0;
};

} /* namespace jsonrpc */
#endif /* JSONRPCAUTHENTICATIONMANAGER_H_ */
