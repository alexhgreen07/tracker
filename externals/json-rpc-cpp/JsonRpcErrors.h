/**
 * @file JsonRpcErrors.h
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcErrors.h
 */

#ifndef JSONRPCERRORS_H_
#define JSONRPCERRORS_H_

#include "json/json.h"
#include <map>
#include <string>

using namespace std;


/**
 * Defines the range for custom Server Error Codes
 * @see http://groups.google.com/group/json-rpc/web/json-rpc-1-2-proposal?pli=1#error-object
 */
#define SPECIFIC_ERROR_START -32000
/**
 * Defines the range for custom Server Error Codes
 * @see http://groups.google.com/group/json-rpc/web/json-rpc-1-2-proposal?pli=1#error-object
 */
#define SPECIFIC_ERROR_STOP -32099

#define KEY_ERROR_CODE "code"
#define KEY_ERROR_MESSAGE "message"

/**
 * Constant to represent, that there is no ERROR.
 */
#define ERROR_NO 0

/**
 * Official JSON-RPC 2.0 Errors
 */
#define ERROR_JSON_PARSE_ERROR -32700
#define ERROR_INVALID_JSON_REQUEST -32600
#define ERROR_METHOD_NOT_FOUND -32601
#define ERROR_INVALID_PARAMS -32602
#define ERROR_INTERNAL_ERROR -32603

/**
 * Specific JSON-RPC 2.0 Errors
 */
#define ERROR_PROCEDURE_IS_METHOD -32604
#define ERROR_PROCEDURE_IS_NOTIFICATION -32605
#define ERROR_PROCEDURE_PARSE_ERROR -32606
#define ERROR_PROCEDURE_POINTER_IS_NULL -32607
#define ERROR_PERMISSION_DENIED -32608
#define ERROR_MALLFORMED_AUTHENTICATION_HEADER -32609
#define ERROR_PARSING_JSON -32610

namespace jsonrpc {


/**
 * This class is implemented as a Singleton and provides a central error handling.
 */
class JsonRpcErrors {

private:
	map<int,string> possibleErrors;


	JsonRpcErrors();

public:

	static JsonRpcErrors* getInstance();

	/**
	 * @param errorCode - the code of the new error, must be between SPECIFIC_ERROR_START and SPECIFIC_ERROR_STOP
	 * @param errorMessage - the string that will be included to the errorBlock
	 * @return true if the errorCode was valid (inside SPECIFIC_ERROR Range), false otherwise.
	 */
	bool addError(int errorCode, string& errorMessage);

	/**
	 * @param errorCode - the errorCode of which you want to create an errorBlock
	 * @return - returns an error Block as described in JSON-RPC 2.0 e.g. \code {"code" : -32600, "message": "INVALID_JSON_REQUEST: The JSON sent is not a valid JSON-RPC Request object" } \endcode
	 * @pre the errorcode must be known, because it is not checked.
	 * @see http://groups.google.com/group/json-rpc/web/json-rpc-1-2-proposal?pli=1#error-object
	 */
	Json::Value getErrorBlock(int errorCode);


};


}

#endif /* JSONRPCERRORS_H_ */
