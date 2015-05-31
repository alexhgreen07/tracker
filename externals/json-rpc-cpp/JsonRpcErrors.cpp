/**
 * @file JsonRpcErrors.cpp
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcErrors.cpp
 */

#include "JsonRpcErrors.h"

namespace jsonrpc {

static JsonRpcErrors* instance = NULL;

JsonRpcErrors::JsonRpcErrors()
{
	//Official Errors
	this->possibleErrors[ERROR_INVALID_JSON_REQUEST] = "INVALID_JSON_REQUEST: The JSON sent is not a valid JSON-RPC Request object";
	this->possibleErrors[ERROR_METHOD_NOT_FOUND] = "METHOD_NOT_FOUND: The method being requested is not available on this server";
	this->possibleErrors[ERROR_INVALID_PARAMS] = "INVALID_PARAMS: Invalid method parameters (invalid name and/or type) recognised";
	this->possibleErrors[ERROR_JSON_PARSE_ERROR] = "JSON_PARSE_ERROR: The JSON-Object is not JSON-Valid";
	this->possibleErrors[ERROR_INTERNAL_ERROR] = "INTERNAL_ERROR: ";


	//Specific Errors
	this->possibleErrors[ERROR_PROCEDURE_IS_METHOD] = "PROCEDURE_IS_METHOD: The requested notification is declared as a method";
	this->possibleErrors[ERROR_PROCEDURE_IS_NOTIFICATION] = "PROCEDURE_IS_NOTIFICATION: The requested method is declared as notification";
	this->possibleErrors[ERROR_PROCEDURE_PARSE_ERROR] = "PROCEDURE_PARSE_ERROR: Something went wrong during parsing a procedure-signature";
	this->possibleErrors[ERROR_PROCEDURE_POINTER_IS_NULL] = "PROCEDURE_POINTER_IS_NULL: Server has no function Reference registered";
	this->possibleErrors[ERROR_PERMISSION_DENIED] = "PERMISSION_DENIED: Insufficient permissions for this request";
	this->possibleErrors[ERROR_MALLFORMED_AUTHENTICATION_HEADER] = "MALLFORMED_AUTHENTICATION_HEADER: Authentication header has wrong format";


}

JsonRpcErrors *JsonRpcErrors::getInstance()
{
	if(instance == NULL) {
		instance = new JsonRpcErrors();
	}
	return instance;
}

bool JsonRpcErrors::addError(int errorCode, string& errorMessage)
{
	if(errorCode < SPECIFIC_ERROR_STOP || errorCode > SPECIFIC_ERROR_START) {
		return false;
	} else {
		this->possibleErrors[errorCode] = errorMessage;
		return true;
	}
}

Json::Value JsonRpcErrors::getErrorBlock(int errorCode)
{
	Json::Value error;
	error[KEY_ERROR_CODE] = errorCode;
	error[KEY_ERROR_MESSAGE] = this->possibleErrors[errorCode];
	return error;
}

}



