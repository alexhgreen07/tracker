/**
 * @file JsonRpcProcedure.cpp
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcProcedure.cpp provides 
 */

#include "JsonRpcProcedure.h"
#include "JsonRpcErrors.h"

namespace jsonrpc {

int JsonRpcProcedure::valdiateParameters(const Json::Value &parameters) {
	map<string, JsonValueType>::iterator it = this->parameters.begin();
	bool ok = true;
	while (ok == true && it != this->parameters.end()) {
		if (!parameters.isMember(it->first.c_str())) {
			ok = false;
		} else {
			switch (it->second) {
			case JSON_STRING:
				if (!parameters[it->first].isString())
					ok = false;
				break;
			case JSON_BOOLEAN:
				if (!parameters[it->first].isBool())
					ok = false;
				break;
			case JSON_INTEGER:
				if (!parameters[it->first].isInt())
					ok = false;
				break;
			case JSON_REAL:
				if (!parameters[it->first].isDouble())
					ok = false;
				break;
			case JSON_OBJECT:
				if (!parameters[it->first].isObject())
					ok = false;
				break;
			case JSON_ARRAY:
				if (!parameters[it->first].isArray())
					ok = false;
				break;
			}
		}
		it++;
	}
	if (ok == true) {
		return ERROR_NO;
	} else {
		return ERROR_INVALID_PARAMS;
	}
}

const map<string, JsonValueType>& JsonRpcProcedure::getParameters() const {
	return this->parameters;
}

JsonProcedureType JsonRpcProcedure::getProcedureType() const {
	return this->procedureType;
}

const string& JsonRpcProcedure::getProcedureName() const {
	return this->procedureName;
}

jsonRequestPointer JsonRpcProcedure::getMethodPointer() {
	if (this->procedureType == RPC_METHOD) {
		return (jsonRequestPointer) this->procedurePointer.rp;
	} else {
		return NULL;
	}
}

jsonNotificationPointer JsonRpcProcedure::getNotificationPointer() {
	if (this->procedureType == RPC_NOTIFICATION) {
		return (jsonNotificationPointer) this->procedurePointer.np;
	} else {
		return NULL;
	}
}

JsonRpcProcedure::JsonRpcProcedure(const string &name,
		const JsonProcedureType procedure_type,
		const map<string, JsonValueType> &parameters) {
	this->procedureName = name;
	this->procedureType = procedure_type;
	this->parameters = parameters;
	this->procedurePointer.np = NULL;
	this->procedurePointer.rp = NULL;
}

JsonRpcProcedure::JsonRpcProcedure(const Json::Value &signature) {
	if (signature.isMember(KEY_PROCEDURE_NAME) && signature.isMember(
			KEY_PROCEDURE_TYPE) && signature.isMember(KEY_PROCEDURE_PARAMETERS)) {
		if (signature[KEY_PROCEDURE_NAME].isString()
				&& signature[KEY_PROCEDURE_TYPE].isBool()
				&& signature[KEY_PROCEDURE_PARAMETERS].isObject()) {
			this->procedureName = signature[KEY_PROCEDURE_NAME].asString();
			if (signature[KEY_PROCEDURE_TYPE].asBool() == JSON_RPC_METHOD) {
				this->procedureType = RPC_METHOD;
			} else {
				this->procedureType = RPC_NOTIFICATION;
			}
			vector<string> parameters =
					signature[KEY_PROCEDURE_PARAMETERS].getMemberNames();
			for (unsigned int i = 0; i < parameters.size(); i++) {
				switch (signature[KEY_PROCEDURE_PARAMETERS][parameters.at(i)].type()) {
				case Json::uintValue:
				case Json::intValue:
					this->parameters[parameters.at(i)] = JSON_INTEGER;
					break;

				case Json::realValue:
					this->parameters[parameters.at(i)] = JSON_REAL;
					break;
				case Json::stringValue:
					this->parameters[parameters.at(i)] = JSON_STRING;
					break;
				case Json::booleanValue:
					this->parameters[parameters.at(i)] = JSON_BOOLEAN;
					break;
				case Json::arrayValue:
					this->parameters[parameters.at(i)] = JSON_ARRAY;
					break;
				case Json::objectValue:
					this->parameters[parameters.at(i)] = JSON_OBJECT;
					break;
				default:
					throw ERROR_PROCEDURE_PARSE_ERROR;
				}
			}
			this->procedurePointer.np = NULL;
			this->procedurePointer.rp = NULL;
		} else {
			throw ERROR_PROCEDURE_PARSE_ERROR;
		}
	} else {
		throw ERROR_PROCEDURE_PARSE_ERROR;
	}
}

bool JsonRpcProcedure::setNotificationPointer(
		jsonNotificationPointer np) {
	if(this->procedureType == RPC_NOTIFICATION) {
		this->procedurePointer.np = np;
		return true;
	} else {
		return false;
	}
}

bool JsonRpcProcedure::setMethodPointer(jsonRequestPointer rp) {
	if(this->procedureType == RPC_METHOD) {
		this->procedurePointer.rp = rp;
		return true;
	} else {
		return false;
	}
}

JsonRpcProcedure::~JsonRpcProcedure() {
}

}

