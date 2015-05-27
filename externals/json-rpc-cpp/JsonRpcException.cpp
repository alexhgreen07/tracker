/**
 * @file JsonRpcException.cpp
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcException.cpp provides 
 */

#include "JsonRpcException.h"

namespace jsonrpc {

JsonRpcException::JsonRpcException(int errorcode, string& message) {
	this->code = errorcode;
	this->errorMessage = message;

}

int JsonRpcException::getCode() const {
	return code;
}

string JsonRpcException::getErrorMessage() const {
	return errorMessage;
}

JsonRpcException::~JsonRpcException() {

}

string JsonRpcException::toString() {
	string str = "ErrorCode: " + this->getCode(); + " -> " + this->getErrorMessage();
	return str;
}

} /* namespace jsonrpc */
