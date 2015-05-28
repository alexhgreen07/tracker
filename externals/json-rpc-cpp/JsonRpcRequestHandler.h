/**
 * @file JsonRpcRequestHandler.h
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcRequestHandler.h provides an interpreter for JSON-RPC Requests.
 *
 */

#ifndef JSONRPCREQUESTHANDLER_H_
#define JSONRPCREQUESTHANDLER_H_

#include "JsonRpcProcedure.h"
#include "JsonRpcAuthenticationManager.h"

#define KEY_REQUEST_METHODNAME "method"
#define KEY_REQUEST_VERSION "jsonrpc"
#define KEY_REQUEST_ID "id"
#define KEY_REQUEST_PARAMETERS "params"
#define KEY_RESPONSE_ERROR "error"
#define KEY_RESPONSE_RESULT "result"
#define KEY_AUTHENTICATION "auth"

#define JSON_RPC_VERSION 2.0

namespace jsonrpc {

/**
 * typedef for observerCallback Functions
 */
typedef void (*observerFunction)(const string&,const Json::Value&);

/**
 * typedef for responseCallback Function
 */
typedef bool (*responseCallBack)(string&);

/**
 * typedef for different Observer Types
 */
typedef enum {
	ON_RESPONSE,
	ON_REQUEST,
	ON_REQUEST_RESPONSE
} JsonRpcObserverType;

/**
 *	This class handles incoming JSON-RPC Requests and reacts according to the JSON-RPC 2.0 protocol.
 *	It's important, that this class doesn't communicate in any way remotely. This is done by JsonRpcConnector, which calls the handleRequest Method of this class
 *	on every new request.
 *	@see JsonRpcConnector
 */
class JsonRpcRequestHandler {
private:
	/**
	 * Each Request Handler has its instancename to identify for logging purposes.
	 */
	string instanceName;

	/**
	 * This vector holds all observers that want to be notified on each request.
	 */
	vector<observerFunction> requestObservers;
	vector<observerFunction> responseObservers;

	/**
	 * This map holds all procedures. The string holds the name of each procedure.
	 */
	map<string,JsonRpcProcedure*> procedures;


	JsonRpcAuthenticationManager* authManager;



	int validateRequest(const Json::Value &val);

	/**
	 * @pre the request must be a valid request
	 * @param request - the request Object compliant to Json-RPC 2.0
	 * @param retValue - a reference to an object which will hold the returnValue afterwards.
	 *
	 * after calling this method, the requested Method will be executed. It is important, that this method only gets called once per request.
	 */
	void processRequest(const Json::Value &request, Json::Value &retValue);

	/**
	 * This method is called on each request, to notify all Registered Observers.
	 */
	void notifyObservers(const vector<observerFunction>& observerGroup, const Json::Value& request);

public:
	/**
	 * @param instanceName - this Constructor sets only the instanceName, for logging and sets the AuthenticationManager to NULL (no AuthManager)
	 */
	JsonRpcRequestHandler(const char* instanceName);

	virtual ~JsonRpcRequestHandler();

	/**
	 * @param fp - the passed function is called on every request that comes in.
	 * @param t - sets the type of this observer.
	 */
	void addObserver(observerFunction fp, JsonRpcObserverType t);
	void removeObserver(observerFunction fp);

	bool addProcedure(JsonRpcProcedure* procedure);
	bool removeProcedure(string procedure);
    JsonRpcAuthenticationManager* getAuthManager() const;
    const string& getInstanceName() const;

    const vector<observerFunction>& getResponseObservers() const;
    const vector<observerFunction>& getRequestObservers() const;

    const map<string,JsonRpcProcedure*>& getProcedures() const;
    void setAuthManager(JsonRpcAuthenticationManager *authManager);
    void setProcedures(map<string,JsonRpcProcedure*> procedures);

    /**
     * This is the key feature of this class, it deals with the JSOn-RPC 2.0 protocol.
     *  @param request - holds (hopefully) a valid JSON-Request Object.
     *  @param retValue a reference to string object which will hold the response after this method;
     */
    void handleRequest(const string& request, string& retValue);

};

} /* namespace jsonrpc */
#endif /* JSONRPCREQUESTHANDLER_H_ */
