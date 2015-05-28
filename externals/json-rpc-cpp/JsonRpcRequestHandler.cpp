/**
 * @file JsonRpcRequestHandler.cpp
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcRequestHandler.cpp provides 
 */

#include "JsonRpcRequestHandler.h"
#include "JsonRpcErrors.h"

namespace jsonrpc {

//TODO: Error Handling
void createErrorBlock(int errorCode, const Json::Value& request, Json::Value& errorBlock) {
	errorBlock[KEY_REQUEST_VERSION] = JSON_RPC_VERSION;
	if (!request.isNull() && request.isMember(KEY_REQUEST_ID)) {
		errorBlock[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
	} else if (request.isNull()) {
		errorBlock[KEY_REQUEST_ID] = Json::Value::null;
	}
	errorBlock[KEY_RESPONSE_ERROR] = JsonRpcErrors::getInstance()->getErrorBlock(
			errorCode);
}

void JsonRpcRequestHandler::processRequest(const Json::Value &request, Json::Value &response) {
	JsonRpcProcedure* method =
			this->procedures[request[KEY_REQUEST_METHODNAME].asString()];
	Json::Value result;
	if (method->getProcedureType() == RPC_METHOD) {
		(*method->getMethodPointer())(request[KEY_REQUEST_PARAMETERS],result);
		//cout << "got result" << endl;
		response[KEY_REQUEST_VERSION] = JSON_RPC_VERSION;
		response[KEY_RESPONSE_RESULT] = result;
		response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
		if(this->authManager != NULL) {
			this->authManager->processAuthentication(request[KEY_AUTHENTICATION],response[KEY_AUTHENTICATION]);
		}
	} else {
		(*method->getNotificationPointer())(request[KEY_REQUEST_PARAMETERS]);
		response = Json::Value::null;
	}
}

int JsonRpcRequestHandler::validateRequest(const Json::Value& request) {
	int error = ERROR_NO;
	JsonRpcProcedure* proc;
	if (!(request.isMember(KEY_REQUEST_METHODNAME)
			&& request.isMember(KEY_REQUEST_VERSION)
			&& request.isMember(KEY_REQUEST_PARAMETERS))) {
		error = ERROR_INVALID_JSON_REQUEST;
	} else {
		map<string, JsonRpcProcedure*>::iterator it = procedures.find(
				request[KEY_REQUEST_METHODNAME].asString());

		if (it != this->procedures.end()) {
			proc = this->procedures[request[KEY_REQUEST_METHODNAME].asString()];
			error = proc->valdiateParameters(request[KEY_REQUEST_PARAMETERS]);
			if (error == ERROR_NO) {
				if (proc->getMethodPointer() == NULL && proc->getNotificationPointer() == NULL) {
					error = ERROR_PROCEDURE_POINTER_IS_NULL;
				} else if (request.isMember(KEY_REQUEST_ID)) {
					if (proc->getProcedureType() == RPC_NOTIFICATION) {
						error = ERROR_PROCEDURE_IS_NOTIFICATION;
					}
				} else if (proc->getProcedureType() == RPC_METHOD) {
					error = ERROR_PROCEDURE_IS_METHOD;
				} if(this->authManager != NULL) {
					error = this->authManager->checkPermission(request[KEY_AUTHENTICATION],proc->getProcedureName());
				}
			}
		} else {
			error = ERROR_METHOD_NOT_FOUND;
		}
	}
	return error;
}

JsonRpcRequestHandler::JsonRpcRequestHandler(const char* instanceName) {
	this->instanceName = instanceName;
	this->authManager = NULL;

	this->requestObservers.clear();
	this->responseObservers.clear();
}

JsonRpcAuthenticationManager *JsonRpcRequestHandler::getAuthManager() const {
	return authManager;
}

const string& JsonRpcRequestHandler::getInstanceName() const {
	return instanceName;
}

const vector<observerFunction>& JsonRpcRequestHandler::getResponseObservers() const {
	return this->responseObservers;
}

const vector<observerFunction>& JsonRpcRequestHandler::getRequestObservers() const {
	return this->requestObservers;
}

const map<string, JsonRpcProcedure*>& JsonRpcRequestHandler::getProcedures() const {
	return procedures;
}

void JsonRpcRequestHandler::setAuthManager(
		JsonRpcAuthenticationManager *authManager) {
	this->authManager = authManager;
}

JsonRpcRequestHandler::~JsonRpcRequestHandler() {
	for (map<string, JsonRpcProcedure*>::iterator it = this->procedures.begin();
			it != this->procedures.end(); it++) {
		delete it->second;
	}
}

void JsonRpcRequestHandler::addObserver(observerFunction fp,
		JsonRpcObserverType t) {
	switch (t) {
	case ON_REQUEST:
		this->requestObservers.push_back(fp);
		break;
	case ON_RESPONSE:
		this->responseObservers.push_back(fp);
		break;
	case ON_REQUEST_RESPONSE:
		this->requestObservers.push_back(fp);
		this->responseObservers.push_back(fp);
		break;
	}
}

void JsonRpcRequestHandler::removeObserver(observerFunction fp) {
	bool found = false;
	unsigned int i = 0;
	while (found == false && i < this->requestObservers.size()) {
		if (this->requestObservers.at(i) == fp) {
			found = true;
			this->requestObservers.erase(this->requestObservers.begin() + i);
		} else {
			i++;
		}
	}
	while (found == false && i < this->responseObservers.size()) {
		if (this->responseObservers.at(i) == fp) {
			found = true;
			this->responseObservers.erase(this->responseObservers.begin() + i);
		} else {
			i++;
		}
	}
}

bool JsonRpcRequestHandler::addProcedure(JsonRpcProcedure *procedure) {
	if (procedure != NULL) {
		this->procedures[procedure->getProcedureName()] = procedure;
		return true;
	} else {
		return false;
	}
}

bool JsonRpcRequestHandler::removeProcedure(string procedure) {
	map<string, JsonRpcProcedure*>::iterator it = this->procedures.find(
			procedure);
	if (it != this->procedures.end()) {
		this->procedures.erase(it);
		return true;
	} else {
		return false;
	}
}

void JsonRpcRequestHandler::setProcedures(
		map<string, JsonRpcProcedure*> procedures) {
	this->procedures = procedures;
}

void JsonRpcRequestHandler::handleRequest(const string& request, string& retValue) {
	Json::Reader reader;
	Json::Value req;
	Json::Value response, resp;
	Json::FastWriter w;
	int error;

//	cout << "Request was: " << request << endl;

	if (reader.parse(request, req)) {
		this->notifyObservers(this->requestObservers, req);
		//It could be a Batch Request
		if (req.isArray()) {
			for (unsigned int i = 0; i < req.size(); i++) {
				error = this->validateRequest(req[i]);
				if (error == ERROR_NO) {
					this->processRequest(req[i],resp);
					if (!resp.isNull()) {
						response[i] = resp;
					}
				} else {
					createErrorBlock(error,req[i],resp);
					response[i] = resp;
				}
			}
			//It could be a simple Request
		} else if (req.isObject()) {
			//cout << "validating request " << endl;
			error = this->validateRequest(req);
			if (error == ERROR_NO) {
				//cout << "processing request" << endl;
				this->processRequest(req, response);
			} else {
				createErrorBlock(error, req, response);
			}
		}
	} else {
		createErrorBlock(ERROR_JSON_PARSE_ERROR,Json::Value::null,response);
	}
	retValue = w.write(response);
	this->notifyObservers(this->requestObservers, req);

}

//TODO: Observer notification
void JsonRpcRequestHandler::notifyObservers(const vector<observerFunction>& obs, const Json::Value& v) {
	for (unsigned int i = 0; i < obs.size(); i++) {
		if(obs.at(i) != NULL)
			(*obs.at(i))(this->instanceName, v);
	}
}

/* namespace jsonrpc */
}

