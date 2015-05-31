/**
 * @file JsonRpcException.h
 * @date 21.07.2011
 * @author Peter Spiess-Knafl
 * @brief JsonRpcException.h
 */

#ifndef JSONRPCEXCEPTION_H_
#define JSONRPCEXCEPTION_H_

#include <string>
using namespace std;

namespace jsonrpc {


class JsonRpcException {
private:
	int code;
	string errorMessage;
public:
	JsonRpcException(int errorcode, string& errorMessage);
	virtual ~JsonRpcException();
    int getCode() const;
    string getErrorMessage() const;

    string toString();



};

} /* namespace jsonrpc */
#endif /* JSONRPCEXCEPTION_H_ */
